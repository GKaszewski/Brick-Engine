#include "ColliderPlacementTool.hpp"

void ColliderPlacementTool::placeCollider() {
	if (vertexAdded < 3) {
		sf::Vertex vertex(game.getWindow().mapPixelToCoords(sf::Mouse::getPosition(game.getWindow())), sf::Color(100, 0, 0, 200));
		auto tileSize = tilemap.getTileSize();
		vertex.position = { roundf(vertex.position.x / tileSize.x) * tileSize.x, roundf(vertex.position.y / tileSize.y) * tileSize.y };
		points.push_back(vertex.position);
		vertexAdded++;
	}
	if (vertexAdded == 3) {
		PhysicsBody collider(nullptr, points[1], PhysicsBody::Type::STATIC);
		collider.setAsTriangle(points);
		collider.createBody();
		colliders.push_back(collider);
		vertexAdded = 0;
		points.clear();
	}
}
