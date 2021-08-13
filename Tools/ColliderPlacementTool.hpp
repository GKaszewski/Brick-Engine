#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "../Entities/PhysicsBody.hpp"
#include "../Graphics/TileMap.hpp"
#include "../Game.hpp"

class ColliderPlacementTool {
public:
	ColliderPlacementTool(Game& game, TileMap& tilemap): game(game), tilemap(tilemap) {}
	void placeCollider();
	void saveCollidersToFile(const std::string & filename);
	std::vector<PhysicsBody> loadCollidersFromFile(const std::string& filename);

	std::vector<PhysicsBody>& Colliders() { return colliders; }
private:
	Game& game;
	TileMap& tilemap;
	std::vector<PhysicsBody> colliders;
	std::vector<sf::Vector2f> points;
	int vertexAdded = 0;
};

