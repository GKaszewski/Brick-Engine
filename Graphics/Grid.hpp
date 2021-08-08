#pragma once
#include <SFML/Graphics.hpp>
class Grid : public sf::Drawable, public sf::Transformable {
public:
	Grid();
	Grid(sf::Vector2u gridSize, sf::Vector2u cellSize, sf::Vector2f position);
	sf::Vector2u& CellSize() { return cellSize; }
	sf::Vector2u& GridSize() { return gridSize; }
	sf::Color& Color() { return color; }

private:
	sf::VertexArray verticies;
	sf::Vector2u gridSize;
	sf::Vector2u cellSize;
	sf::Vector2f position;
	sf::Color color;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void generateGrid();
};

