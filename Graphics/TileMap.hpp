#pragma once
#include <SFML/Graphics.hpp>
class TileMap : public sf::Drawable, public sf::Transformable {
public:
	bool load(const sf::Texture* tilesetTexture, sf::Vector2u textureTileSize, sf::Vector2u tileSize ,const int* tiles, unsigned int width, unsigned int height);
	//void setPosition(sf::Vector2f position);

	const sf::Vector2u& getTileSize() { return tileSize; }
private:
	sf::VertexArray verticies;
	const sf::Texture* tileset;
	unsigned int width;
	unsigned int height;
	sf::Vector2u textureTileSize;
	sf::Vector2u tileSize;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

