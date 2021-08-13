#include "TileMap.hpp"

bool TileMap::load(const sf::Texture* tilesetTexture, sf::Vector2u textureTileSize, sf::Vector2u tileSize, const int* tiles, unsigned int width, unsigned int height) {
    if (tilesetTexture == nullptr) return false;
    tileset = tilesetTexture;

    this->textureTileSize = textureTileSize;
    this->tileSize = tileSize;
    this->width = width;
    this->height = height;

    verticies.setPrimitiveType(sf::Quads);
    verticies.resize(4 * width * height);
    for (unsigned int i = 0; i < width; ++i) {
        for (unsigned int j = 0; j < height; ++j) {
            auto tileNumber = tiles[i + j * width];
            int tu = -1;
            int tv = -1;
            if (tileNumber >= 0) {
                tu = tileNumber % (tileset->getSize().x / textureTileSize.x);
                tv = tileNumber / (tileset->getSize().x / textureTileSize.x);
            }

            sf::Vertex* quad = &verticies[(i + j * width) * 4];
            quad[0].position = sf::Vector2f(i * tileSize.x, j * tileSize.y);
            quad[1].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y);
            quad[2].position = sf::Vector2f((i + 1) * tileSize.x, (j + 1) * tileSize.y);
            quad[3].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y);

            quad[0].texCoords = sf::Vector2f(tu * textureTileSize.x, tv * textureTileSize.y);
            quad[1].texCoords = sf::Vector2f((tu + 1) * textureTileSize.x, tv * textureTileSize.y);
            quad[2].texCoords = sf::Vector2f((tu + 1) * textureTileSize.x, (tv + 1) * textureTileSize.y);
            quad[3].texCoords = sf::Vector2f(tu * textureTileSize.x, (tv + 1) * textureTileSize.y);
        }
    }

    return true;
}

void TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();
    states.texture = tileset;
    target.draw(verticies, states);
}
