#include "CollisionListener.hpp"
#include "../Managers/PhysicsManager.hpp"
#include "../Entities/PhysicsBody.hpp"
#include <iostream>

void CollisionListener::BeginContact(b2Contact* contactInfo) {
	auto first = (PhysicsBody*)contactInfo->GetFixtureA()->GetBody()->GetUserData().pointer;
	auto second = (PhysicsBody*)contactInfo->GetFixtureB()->GetBody()->GetUserData().pointer;

	first->onCollisionStart(second);
}

void CollisionListener::EndContact(b2Contact* contactInfo) {
	auto first = (PhysicsBody*)contactInfo->GetFixtureA()->GetBody()->GetUserData().pointer;
	auto second = (PhysicsBody*)contactInfo->GetFixtureB()->GetBody()->GetUserData().pointer;

	first->onCollisionEnd(second);
}
