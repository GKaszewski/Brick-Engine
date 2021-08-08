#include "Grid.hpp"

Grid::Grid() {
	gridSize = sf::Vector2u(1280, 704);
	cellSize = sf::Vector2u(64, 64);
	position = sf::Vector2f(0.0f, 0.0f);
	color = sf::Color(sf::Color::White);

	generateGrid();
}

Grid::Grid(sf::Vector2u gridSize, sf::Vector2u cellSize, sf::Vector2f position) {
	this->gridSize = gridSize;
	this->cellSize = cellSize;
	this->position = position;
	color = sf::Color(sf::Color::White);

	generateGrid();
}

void Grid::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform *= getTransform();
	target.draw(verticies, states);
}

void Grid::generateGrid() {
    verticies.setPrimitiveType(sf::LineStrip);
    verticies.resize(4 * gridSize.x * gridSize.y);
    for (unsigned int i = 0; i < gridSize.x; ++i) {
        for (unsigned int j = 0; j < gridSize.y; ++j) {

            sf::Vertex* quad = &verticies[(i + j * gridSize.x) * 4];
            quad[0].position = sf::Vector2f(i * cellSize.x, j * cellSize.y);
            quad[1].position = sf::Vector2f((i + 1) * cellSize.x, j * cellSize.y);
            quad[2].position = sf::Vector2f((i + 1) * cellSize.x, (j + 1) * cellSize.y);
            quad[3].position = sf::Vector2f(i * cellSize.x, (j + 1) * cellSize.y);

			quad[0].color = color;
			quad[1].color = color;
			quad[2].color = color;
			quad[3].color = color;
        }
    }
}
