#pragma once
#include <tmxlite/Map.hpp>
#include <tmxlite/TileLayer.hpp>
#include <tmxlite/detail/Log.hpp>
#include <SFML/Graphics.hpp>

#include <memory>
#include <vector>
#include <array>
#include <map>
#include <string>
#include <limits>
#include <iostream>
#include <cmath>

class TmxMapLayer final : public sf::Drawable {
public:
	TmxMapLayer(const tmx::Map& map, std::size_t idx);
	~TmxMapLayer() = default;
	
	TmxMapLayer(const TmxMapLayer&) = delete;
	TmxMapLayer operator = (const TmxMapLayer&) = delete;

	const sf::FloatRect getGlobalBounds() const { return globalBounds; }

	void setTile(int tileX, int tileY, tmx::TileLayer::Tile tile, bool refresh = true);
	tmx::TileLayer::Tile getTile(int tileX, int tileY);
	void setColor(int tileX, int tileY, sf::Color color, bool refresh = true);
	sf::Color getColor(int tileX, int tileY);
	void update(sf::Time elapsed);

private:
	sf::Vector2f chunkSize = sf::Vector2f(512.f, 512.f);
	sf::Vector2u chunkCount;
	sf::Vector2u mapTileSize;
	sf::FloatRect globalBounds;

	using TextureResource = std::map<std::string, std::unique_ptr<sf::Texture>>;
	TextureResource textureResource;

	struct AnimationState {
		sf::Vector2u tileCords;
		sf::Time startTime;
		sf::Time currentTime;
		tmx::Tileset::Tile animTile;
		std::uint8_t flipFlags;
	};

	class Chunk	final : public sf::Transformable, public sf::Drawable {
	public:
		using Ptr = std::unique_ptr<Chunk>;
		using Tile = std::array<sf::Vertex, 4u>;

		Chunk(const tmx::TileLayer& layer, std::vector<const tmx::Tileset*> tilesets, const sf::Vector2f& position, const sf::Vector2f& tileCount, const sf::Vector2u& tileSize,
			std::size_t rowSize, TextureResource& tr, const std::map<std::uint32_t, tmx::Tileset::Tile>& animTiles);
		~Chunk() = default;
		Chunk(const Chunk&) = delete;
		Chunk& operator = (const Chunk&) = delete;
		std::vector<AnimationState>& getActiveAnimations() { return activeAnimations; }

		void generateTiles(bool registerAnimation = false);
		
		tmx::TileLayer::Tile getTile(int x, int y) const;
		void setTile(int x, int y, tmx::TileLayer::Tile tile, bool refresh);
		sf::Color getColor(int x, int y) const;
		void setColor(int x, int y, sf::Color color, bool refresh);
		void maybeRegenerate(bool refresh);
		int calcIndexFrom(int x, int y) const;
		bool empty() const { return chunkArrays.empty(); }
		void flipY(sf::Vector2f* v0, sf::Vector2f* v1, sf::Vector2f* v2, sf::Vector2f* v3);
		void flipX(sf::Vector2f* v0, sf::Vector2f* v1, sf::Vector2f* v2, sf::Vector2f* v3);
		void flipD(sf::Vector2f* v0, sf::Vector2f* v1, sf::Vector2f* v2, sf::Vector2f* v3);
		void doFlips(std::uint8_t bits, sf::Vector2f* v0, sf::Vector2f* v1, sf::Vector2f* v2, sf::Vector2f* v3);

	private:
		class ChunkArray final : public sf::Drawable {
		public:
			using Ptr = std::unique_ptr<ChunkArray>;
			tmx::Vector2u tileSetSize;
			sf::Vector2u tsTileCount;
			std::uint32_t firstGID, lastGID;
			
			explicit ChunkArray(const sf::Texture& t, const tmx::Tileset& ts);
			~ChunkArray() = default;
			ChunkArray(const ChunkArray&) = delete;
			ChunkArray& operator = (const ChunkArray&) = delete;

			void reset();
			void addTile(const Chunk::Tile& tile);
			sf::Vector2u getTextureSize() const { return texture.getSize(); }
		
		private:
			const sf::Texture& texture;
			std::vector<sf::Vertex> vertices;
			void draw(sf::RenderTarget& rt, sf::RenderStates states) const override;
		};

		sf::Uint8 layerOpacity;     // opacity of the layer
		sf::Vector2f layerOffset;   // Layer offset
		sf::Vector2u mapTileSize;   // general Tilesize of Map
		sf::Vector2f chunkTileCount;   // chunk tilecount
		std::vector<tmx::TileLayer::Tile> chunkTileIDs; // stores all tiles in this chunk for later manipulation
		std::vector<sf::Color> chunkColors; // stores colors for extended color effects
		std::map<std::uint32_t, tmx::Tileset::Tile> animTiles;    // animation catalogue
		std::vector<AnimationState> activeAnimations;     // Animations to be done in this chunk
		std::vector<ChunkArray::Ptr> chunkArrays;
		void draw(sf::RenderTarget& rt, sf::RenderStates states) const override;
	};

	std::vector<Chunk::Ptr> chunks;
	mutable std::vector<Chunk*> visibleChunks;
	
	Chunk::Ptr& getChunkAndTransform(int x, int y, sf::Vector2u& chunkRelative) {
		uint32_t chunkX = floor((x * mapTileSize.x) / chunkSize.x);
		uint32_t chunkY = floor((y * mapTileSize.y) / chunkSize.y);
		chunkRelative.x = ((x * mapTileSize.x) - chunkX * chunkSize.x) / mapTileSize.x;
		chunkRelative.y = ((y * mapTileSize.y) - chunkY * chunkSize.y) / mapTileSize.y;
		return  chunks[chunkX + chunkY * chunkCount.x];
	}

	void createChunks(const tmx::Map& map, const tmx::TileLayer& layer);
	void updateVisibility(const sf::View& view) const;
	void draw(sf::RenderTarget& rt, sf::RenderStates states) const override;
};

