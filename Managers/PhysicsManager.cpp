#include "PhysicsManager.hpp"

PhysicsManager* PhysicsManager::instance = 0;

PhysicsManager* PhysicsManager::getInstance()
{
    if (instance == 0) {
        instance = new PhysicsManager();
    }

    return instance;
}

PhysicsManager::PhysicsManager() {
    gravity = new b2Vec2(0.0, 10.0);
    world = new b2World(*gravity);
}

PhysicsManager::~PhysicsManager() {
    delete gravity;
    delete world;
    delete instance;
}

