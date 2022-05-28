#pragma once
#include <set>
#include <vector>
#include "../Entities/PhysicsBody.hpp"

class EntityManager {
public:
	static EntityManager* getInstance();

	std::set<PhysicsBody*> physicsBodiesToRemove;
	std::vector<PhysicsBody*> physicsEntities;

	void destroyBodies();

private:
	static EntityManager* instance;
	EntityManager();
	~EntityManager();
};

