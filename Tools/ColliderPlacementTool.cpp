#include "ColliderPlacementTool.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

#include "../utils.hpp"

void ColliderPlacementTool::placeCollider() {
	if (vertexAdded < 3) {
		sf::Vertex vertex(game.getWindow().mapPixelToCoords(sf::Mouse::getPosition(game.getWindow())), sf::Color(100, 0, 0, 200));
		vertex.position = { roundf(vertex.position.x / tileSize.x) * tileSize.x, roundf(vertex.position.y / tileSize.y) * tileSize.y };
		points.push_back(vertex.position);
		vertexAdded++;
	}
	if (vertexAdded == 3) {
		auto collider = new PhysicsBody(nullptr, points[1], PhysicsBody::Type::STATIC);
		collider->setAsTriangle(points);
		collider->createBody();
		colliders.push_back(collider);
		vertexAdded = 0;
		points.clear();
	}
}

void ColliderPlacementTool::saveCollidersToFile(const std::string& filename) {
	std::ofstream file;
	file.open(filename);
	for (auto collider : colliders) {
		auto pointCount = collider->Shape().getPointCount();
		for (int i = 0; i < pointCount; ++i) {
			auto point = collider->Shape().getTransform().transformPoint(collider->Shape().getPoint(i));
			if (i < (pointCount - 1))
				file << point.x << "," << point.y << ",";
			else
				file << point.x << "," << point.y;
		}
		file << "\n";
	}

	file.close();
}

std::vector<PhysicsBody*> ColliderPlacementTool::loadCollidersFromFile(const std::string& filename) {
	std::string line;
	std::ifstream file(filename);
	std::vector<PhysicsBody*> loadedColliders;
	while (std::getline(file, line)) {
		auto parts = utils::split(line, ',');
		std::vector<sf::Vector2f> points;
		for (int i = 0; i < parts.size(); i += 2) {
			auto x = std::stof(parts[i]);
			auto y = std::stof(parts[i+1]);
			points.push_back({ x,y });
		}
		
		auto collider = new PhysicsBody(nullptr, points[1], PhysicsBody::Type::STATIC);
		collider->setAsTriangle(points);
		collider->createBody();
		loadedColliders.push_back(collider);
	}

	return loadedColliders;
}
