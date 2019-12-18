#pragma once
#include <SFML/Graphics.hpp>

enum CellType {
	BACKGROUND,
	COLLECTABLE,
	SOLID,
	TRAP
};

class Cell {
public:
	Cell(sf::Vector2f pos = { 0,0 }) {
		size = { 64, 64 };
		pos = { pos.x * size.x, pos.y * size.y };

		shape.setSize(size);
		shape.setPosition(pos);

		shape.setFillColor(sf::Color::White);
		shape.setOutlineColor(sf::Color::Black);
		shape.setOutlineThickness(2);

		type = CellType::BACKGROUND;
	}

	void changeColor(sf::Color color = sf::Color::Green) {
		shape.setFillColor(color);
		shape.setOutlineThickness(1);
	}

	void changeTexture(sf::Texture* texture) {
		if (texture == nullptr) hasTexture = false;
		else hasTexture = true;
		shape.setTexture(texture);
		shape.setOutlineThickness(0);
	}

	void changeTexture(sf::Texture* texture, int tile) {
		shape.setFillColor(sf::Color::White);
		shape.setOutlineThickness(0);
		sf::IntRect tilePosition(0, 0, 16, 16);
		tilePosition.left += (16 * tile);
		shape.setTextureRect(tilePosition);
		shape.setTexture(texture);
		hasTexture = true;
		if (tile == 2) type = CellType::BACKGROUND;
		else if (tile == 0) type = CellType::SOLID;
		else if (tile == 1) type = CellType::SOLID;
		else if (tile == 3) type = CellType::TRAP;
		else if (tile == 4) type = CellType::COLLECTABLE;
		else if (tile == 5) type = CellType::COLLECTABLE;
	}

	void placePlayer() {
		shape.setFillColor(sf::Color::Red);
	}

	bool checkMouseClick(sf::Vector2f position) {
		if (shape.getGlobalBounds().contains(position)) {
			return true;
		}

		return false;
	}

	sf::RectangleShape shape;
	bool hasTexture = false;
	CellType type;
private:
	sf::Vector2f position;
	sf::Vector2f size;
};