#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "../Entities/PhysicsBody.hpp"
#include "../Graphics/TileMap.hpp"
#include "../Core/Game.hpp"

class ColliderPlacementTool {
public:
	ColliderPlacementTool(Game& game, const sf::Vector2u& tileSize) : game(game) { this->tileSize = tileSize; }
	void placeCollider();
	void saveCollidersToFile(const std::string & filename);
	std::vector<PhysicsBody> loadCollidersFromFile(const std::string& filename);

	std::vector<PhysicsBody>& Colliders() { return colliders; }
	sf::Vector2u& TileSize() { return tileSize; }
private:
	Game& game;
	sf::Vector2u tileSize;
	std::vector<PhysicsBody> colliders;
	std::vector<sf::Vector2f> points;
	int vertexAdded = 0;
};

