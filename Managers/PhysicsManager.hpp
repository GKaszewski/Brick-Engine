#pragma once
#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
class PhysicsManager {
public:
	static PhysicsManager* getInstance();
	static constexpr double SCALE = 30.0;
	b2Vec2 * gravity;
	b2World * world;
private:
	static PhysicsManager* instance;
	PhysicsManager();
	~PhysicsManager();
};