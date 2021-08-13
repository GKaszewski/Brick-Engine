#include "TmxMap.hpp"

TmxMapLayer::TmxMapLayer(const tmx::Map& map, std::size_t idx) {
    const auto& layers = map.getLayers();
    if (map.getOrientation() == tmx::Orientation::Orthogonal &&
        idx < layers.size() && layers[idx]->getType() == tmx::Layer::Type::Tile) {
        //round the chunk size to the nearest tile
        const auto tileSize = map.getTileSize();
        chunkSize.x = std::floor(chunkSize.x / tileSize.x) * tileSize.x;
        chunkSize.y = std::floor(chunkSize.y / tileSize.y) * tileSize.y;
        mapTileSize.x = map.getTileSize().x;
        mapTileSize.y = map.getTileSize().y;
        const auto& layer = layers[idx]->getLayerAs<tmx::TileLayer>();
        createChunks(map, layer);

        auto mapSize = map.getBounds();
        globalBounds.width = mapSize.width;
        globalBounds.height = mapSize.height;
    } else {
        std::cout << "Not a valid orthogonal layer, nothing will be drawn." << std::endl;
    }
}

void TmxMapLayer::setTile(int tileX, int tileY, tmx::TileLayer::Tile tile, bool refresh) {
    sf::Vector2u chunkLocale;
    const auto& selectedChunk = getChunkAndTransform(tileX, tileY, chunkLocale);
    selectedChunk->setTile(chunkLocale.x, chunkLocale.y, tile, refresh);
}

tmx::TileLayer::Tile TmxMapLayer::getTile(int tileX, int tileY) {
    sf::Vector2u chunkLocale;
    const auto& selectedChunk = getChunkAndTransform(tileX, tileY, chunkLocale);
    return selectedChunk->getTile(chunkLocale.x, chunkLocale.y);
}

void TmxMapLayer::setColor(int tileX, int tileY, sf::Color color, bool refresh) {
    sf::Vector2u chunkLocale;
    const auto& selectedChunk = getChunkAndTransform(tileX, tileY, chunkLocale);
    selectedChunk->setColor(chunkLocale.x, chunkLocale.y, color, refresh);
}

sf::Color TmxMapLayer::getColor(int tileX, int tileY) {
    sf::Vector2u chunkLocale;
    const auto& selectedChunk = getChunkAndTransform(tileX, tileY, chunkLocale);
    return selectedChunk->getColor(chunkLocale.x, chunkLocale.y);
}

void TmxMapLayer::update(sf::Time elapsed) {
    for (auto& c : visibleChunks) {
        for (AnimationState& as : c->getActiveAnimations()) {
            as.currentTime += elapsed;

            tmx::TileLayer::Tile tile;
            tile.ID = as.animTile.animation.frames[0].tileID;
            tile.flipFlags = 0; // TODO: get flipFlags from original tmx::TileLayer::Tile

            std::uint32_t animTime = 0;
            for (const auto& frame : as.animTile.animation.frames) {
                animTime += frame.duration;
                if (as.currentTime.asMilliseconds() >= animTime) {
                    tile.ID = frame.tileID;
                    if (frame == as.animTile.animation.frames.back()) {
                        as.currentTime = sf::milliseconds(0);
                    }
                }
            }

            setTile(as.tileCords.x, as.tileCords.y, tile);
        }
    }
}

void TmxMapLayer::createChunks(const tmx::Map& map, const tmx::TileLayer& layer) {
    //look up all the tile sets and load the textures
    const auto& tileSets = map.getTilesets();
    const auto& layerIDs = layer.getTiles();
    std::uint32_t maxID = std::numeric_limits<std::uint32_t>::max();
    std::vector<const tmx::Tileset*> usedTileSets;

    for (auto i = tileSets.rbegin(); i != tileSets.rend(); ++i) {
        for (const auto& tile : layerIDs) {
            if (tile.ID >= i->getFirstGID() && tile.ID < maxID) {
                usedTileSets.push_back(&(*i));
                break;
            }
        }
        maxID = i->getFirstGID();
    }

    sf::Image fallback;
    fallback.create(2, 2, sf::Color::Magenta);
    for (const auto& ts : usedTileSets) {
        const auto& path = ts->getImagePath();
        std::unique_ptr<sf::Texture> newTexture = std::make_unique<sf::Texture>();
        sf::Image img;
        if (!img.loadFromFile(path)) {
            newTexture->loadFromImage(fallback);
        } else {
            if (ts->hasTransparency()) {
                auto transparency = ts->getTransparencyColour();
                img.createMaskFromColor({ transparency.r, transparency.g, transparency.b, transparency.a });
            }
            newTexture->loadFromImage(img);
        }
        textureResource.insert(std::make_pair(path, std::move(newTexture)));
    }

    //calculate the number of chunks in the layer
    //and create each one
    const auto bounds = map.getBounds();
    chunkCount.x = static_cast<sf::Uint32>(std::ceil(bounds.width / chunkSize.x));
    chunkCount.y = static_cast<sf::Uint32>(std::ceil(bounds.height / chunkSize.y));

    sf::Vector2u tileSize(map.getTileSize().x, map.getTileSize().y);

    for (auto y = 0u; y < chunkCount.y; ++y) {
        sf::Vector2f tileCount(chunkSize.x / tileSize.x, chunkSize.y / tileSize.y);
        for (auto x = 0u; x < chunkCount.x; ++x) {
            // calculate size of each Chunk (clip against map)
            if ((x + 1) * chunkSize.x > bounds.width) {
                tileCount.x = (bounds.width - x * chunkSize.x) / map.getTileSize().x;
            }
            if ((y + 1) * chunkSize.y > bounds.height) {
                tileCount.y = (bounds.height - y * chunkSize.y) / map.getTileSize().y;
            }
            chunks.emplace_back(std::make_unique<Chunk>(layer, usedTileSets,
                sf::Vector2f(x * chunkSize.x, y * chunkSize.y), tileCount, tileSize, map.getTileCount().x, textureResource, map.getAnimatedTiles()));
        }
    }
}

void TmxMapLayer::updateVisibility(const sf::View& view) const {
    sf::Vector2f viewCorner = view.getCenter();
    viewCorner -= view.getSize() / 2.f;

    int posX = static_cast<int>(std::floor(viewCorner.x / chunkSize.x));
    int posY = static_cast<int>(std::floor(viewCorner.y / chunkSize.y));
    int posX2 = static_cast<int>(std::ceil((viewCorner.x + view.getSize().x) / chunkSize.x));
    int posY2 = static_cast<int>(std::ceil((viewCorner.y + view.getSize().x) / chunkSize.y));

    std::vector<Chunk*> visible;
    for (auto y = posY; y < posY2; ++y) {
        for (auto x = posX; x < posX2; ++x) {
            std::size_t idx = y * int(chunkCount.x) + x;
            if (idx >= 0u && idx < chunks.size() && !chunks[idx]->empty()) {
                visible.push_back(chunks[idx].get());
            }
        }
    }

    std::swap(visibleChunks, visible);
}

void TmxMapLayer::draw(sf::RenderTarget& rt, sf::RenderStates states) const {
    updateVisibility(rt.getView());
    for (const auto& c : visibleChunks)
    {
        rt.draw(*c, states);
    }
}

TmxMapLayer::Chunk::Chunk(const tmx::TileLayer& layer, std::vector<const tmx::Tileset*> tilesets, const sf::Vector2f& position, const sf::Vector2f& tileCount, const sf::Vector2u& tileSize, std::size_t rowSize, TextureResource& tr, const std::map<std::uint32_t, tmx::Tileset::Tile>& animTiles) : animTiles(animTiles) {
    setPosition(position);
    layerOpacity = static_cast<sf::Uint8>(layer.getOpacity() / 1.f * 255.f);
    sf::Color vertColour = sf::Color(200, 200, 200, layerOpacity);
    auto offset = layer.getOffset();
    layerOffset.x = offset.x;
    layerOffset.x = offset.y;
    chunkTileCount.x = tileCount.x;
    chunkTileCount.y = tileCount.y;
    mapTileSize = tileSize;
    const auto& tileIDs = layer.getTiles();

    //go through the tiles and create all arrays (for latter manipulation)
    for (const auto& ts : tilesets) {
        if (ts->getImagePath().empty()) {
            tmx::Logger::log("This example does not support Collection of Images tilesets", tmx::Logger::Type::Info);
            tmx::Logger::log("Chunks using " + ts->getName() + " will not be created", tmx::Logger::Type::Info);
            continue;
        }
        chunkArrays.emplace_back(std::make_unique<ChunkArray>(*tr.find(ts->getImagePath())->second, *ts));
    }
    std::size_t xPos = static_cast<std::size_t>(position.x / tileSize.x);
    std::size_t yPos = static_cast<std::size_t>(position.y / tileSize.y);
    for (auto y = yPos; y < yPos + tileCount.y; ++y) {
        for (auto x = xPos; x < xPos + tileCount.x; ++x) {
            auto idx = (y * rowSize + x);
            chunkTileIDs.emplace_back(tileIDs[idx]);
            chunkColors.emplace_back(vertColour);
        }
    }
    generateTiles(true);
}

void TmxMapLayer::Chunk::generateTiles(bool registerAnimation) {
    if (registerAnimation) {
        activeAnimations.clear();
    }
    for (const auto& ca : chunkArrays) {
        sf::Uint32 idx = 0;
        std::size_t xPos = static_cast<std::size_t>(getPosition().x / mapTileSize.x);
        std::size_t yPos = static_cast<std::size_t>(getPosition().y / mapTileSize.y);
        for (auto y = yPos; y < yPos + chunkTileCount.y; ++y)
        {
            for (auto x = xPos; x < xPos + chunkTileCount.x; ++x)
            {
                if (idx < chunkTileIDs.size() && chunkTileIDs[idx].ID >= ca->firstGID
                    && chunkTileIDs[idx].ID <= ca->lastGID)
                {
                    if (registerAnimation && animTiles.find(chunkTileIDs[idx].ID) != animTiles.end())
                    {
                        AnimationState as;
                        as.animTile = animTiles[chunkTileIDs[idx].ID];
                        as.startTime = sf::milliseconds(0);
                        as.tileCords = sf::Vector2u(x, y);
                        activeAnimations.push_back(as);
                    }

                    sf::Vector2f tileOffset(x * mapTileSize.x, (float)y * mapTileSize.y + mapTileSize.y - ca->tileSetSize.y);

                    auto idIndex = chunkTileIDs[idx].ID - ca->firstGID;
                    sf::Vector2f tileIndex(idIndex % ca->tsTileCount.x, idIndex / ca->tsTileCount.x);
                    tileIndex.x *= ca->tileSetSize.x;
                    tileIndex.y *= ca->tileSetSize.y;
                    Tile tile =
                    {
                                sf::Vertex(tileOffset - getPosition(), chunkColors[idx], tileIndex),
                                sf::Vertex(tileOffset - getPosition() + sf::Vector2f(ca->tileSetSize.x, 0.f), chunkColors[idx], tileIndex + sf::Vector2f(ca->tileSetSize.x, 0.f)),
                                sf::Vertex(tileOffset - getPosition() + sf::Vector2f(ca->tileSetSize.x, ca->tileSetSize.y), chunkColors[idx], tileIndex + sf::Vector2f(ca->tileSetSize.x, ca->tileSetSize.y)),
                                sf::Vertex(tileOffset - getPosition() + sf::Vector2f(0.f, ca->tileSetSize.y), chunkColors[idx], tileIndex + sf::Vector2f(0.f, ca->tileSetSize.y))
                    };
                    doFlips(chunkTileIDs[idx].flipFlags, &tile[0].texCoords, &tile[1].texCoords, &tile[2].texCoords, &tile[3].texCoords);
                    ca->addTile(tile);
                }
                idx++;
            }
        }
    }
}

tmx::TileLayer::Tile TmxMapLayer::Chunk::getTile(int x, int y) const {
    return chunkTileIDs[calcIndexFrom(x, y)];
}

void TmxMapLayer::Chunk::setTile(int x, int y, tmx::TileLayer::Tile tile, bool refresh) {
    chunkTileIDs[calcIndexFrom(x, y)] = tile;
    maybeRegenerate(refresh);
}

sf::Color TmxMapLayer::Chunk::getColor(int x, int y) const {
    return chunkColors[calcIndexFrom(x, y)];
}

void TmxMapLayer::Chunk::setColor(int x, int y, sf::Color color, bool refresh) {
    chunkColors[calcIndexFrom(x, y)] = color;
    maybeRegenerate(refresh);
}

void TmxMapLayer::Chunk::maybeRegenerate(bool refresh) {
    if (refresh) {
        for (const auto& ca : chunkArrays) {
            ca->reset();
        }
        generateTiles();
    }
}

int TmxMapLayer::Chunk::calcIndexFrom(int x, int y) const {
    return x + y * chunkTileCount.x;
}

void TmxMapLayer::Chunk::flipY(sf::Vector2f* v0, sf::Vector2f* v1, sf::Vector2f* v2, sf::Vector2f* v3) {
    //Flip Y
    sf::Vector2f tmp = *v0;
    v0->y = v2->y;
    v1->y = v2->y;
    v2->y = tmp.y;
    v3->y = v2->y;
}

void TmxMapLayer::Chunk::flipX(sf::Vector2f* v0, sf::Vector2f* v1, sf::Vector2f* v2, sf::Vector2f* v3) {
    //Flip X
    sf::Vector2f tmp = *v0;
    v0->x = v1->x;
    v1->x = tmp.x;
    v2->x = v3->x;
    v3->x = v0->x;
}

void TmxMapLayer::Chunk::flipD(sf::Vector2f* v0, sf::Vector2f* v1, sf::Vector2f* v2, sf::Vector2f* v3) {
    //Diagonal flip
    sf::Vector2f tmp = *v1;
    v1->x = v3->x;
    v1->y = v3->y;
    v3->x = tmp.x;
    v3->y = tmp.y;
}

void TmxMapLayer::Chunk::doFlips(std::uint8_t bits, sf::Vector2f* v0, sf::Vector2f* v1, sf::Vector2f* v2, sf::Vector2f* v3) {
    //0000 = no change
            //0100 = vertical = swap y axis
            //1000 = horizontal = swap x axis
            //1100 = horiz + vert = swap both axes = horiz+vert = rotate 180 degrees
            //0010 = diag = rotate 90 degrees right and swap x axis
            //0110 = diag+vert = rotate 270 degrees right
            //1010 = horiz+diag = rotate 90 degrees right
            //1110 = horiz+vert+diag = rotate 90 degrees right and swap y axis
    if (!(bits & tmx::TileLayer::FlipFlag::Horizontal) &&
        !(bits & tmx::TileLayer::FlipFlag::Vertical) &&
        !(bits & tmx::TileLayer::FlipFlag::Diagonal)) {
        //Shortcircuit tests for nothing to do
        return;
    }
    else if (!(bits & tmx::TileLayer::FlipFlag::Horizontal) &&
        (bits & tmx::TileLayer::FlipFlag::Vertical) &&
        !(bits & tmx::TileLayer::FlipFlag::Diagonal)) {
        //0100
        flipY(v0, v1, v2, v3);
    }
    else if ((bits & tmx::TileLayer::FlipFlag::Horizontal) &&
        !(bits & tmx::TileLayer::FlipFlag::Vertical) &&
        !(bits & tmx::TileLayer::FlipFlag::Diagonal)) {
        //1000
        flipX(v0, v1, v2, v3);
    }
    else if ((bits & tmx::TileLayer::FlipFlag::Horizontal) &&
        (bits & tmx::TileLayer::FlipFlag::Vertical) &&
        !(bits & tmx::TileLayer::FlipFlag::Diagonal)) {
        //1100
        flipY(v0, v1, v2, v3);
        flipX(v0, v1, v2, v3);
    }
    else if (!(bits & tmx::TileLayer::FlipFlag::Horizontal) &&
        !(bits & tmx::TileLayer::FlipFlag::Vertical) &&
        (bits & tmx::TileLayer::FlipFlag::Diagonal)) {
        //0010
        flipD(v0, v1, v2, v3);
    }
    else if (!(bits & tmx::TileLayer::FlipFlag::Horizontal) &&
        (bits & tmx::TileLayer::FlipFlag::Vertical) &&
        (bits & tmx::TileLayer::FlipFlag::Diagonal)) {
        //0110
        flipX(v0, v1, v2, v3);
        flipD(v0, v1, v2, v3);
    }
    else if ((bits & tmx::TileLayer::FlipFlag::Horizontal) &&
        !(bits & tmx::TileLayer::FlipFlag::Vertical) &&
        (bits & tmx::TileLayer::FlipFlag::Diagonal)) {
        //1010
        flipY(v0, v1, v2, v3);
        flipD(v0, v1, v2, v3);
    }
    else if ((bits & tmx::TileLayer::FlipFlag::Horizontal) &&
        (bits & tmx::TileLayer::FlipFlag::Vertical) &&
        (bits & tmx::TileLayer::FlipFlag::Diagonal)) {
        //1110
        flipY(v0, v1, v2, v3);
        flipX(v0, v1, v2, v3);
        flipD(v0, v1, v2, v3);
    }
}

void TmxMapLayer::Chunk::draw(sf::RenderTarget& rt, sf::RenderStates states) const {
    states.transform *= getTransform();
    for (const auto& a : chunkArrays) {
        rt.draw(*a, states);
    }
}

TmxMapLayer::Chunk::ChunkArray::ChunkArray(const sf::Texture& t, const tmx::Tileset& ts) : texture(t) {
    auto texSize = getTextureSize();
    tileSetSize = ts.getTileSize();
    tsTileCount.x = texSize.x / tileSetSize.x;
    tsTileCount.y = texSize.y / tileSetSize.y;
    firstGID = ts.getFirstGID();
    lastGID = ts.getLastGID();
}

void TmxMapLayer::Chunk::ChunkArray::reset() {
    vertices.clear();
}

void TmxMapLayer::Chunk::ChunkArray::addTile(const Chunk::Tile& tile) {
    for (const auto& v : tile) {
        vertices.push_back(v);
    }
}

void TmxMapLayer::Chunk::ChunkArray::draw(sf::RenderTarget& rt, sf::RenderStates states) const {
    states.texture = &texture;
    rt.draw(vertices.data(), vertices.size(), sf::Quads, states);
}
