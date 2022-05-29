#include "CollisionListener.hpp"
#include "../Managers/PhysicsManager.hpp"
#include "../Entities/PhysicsBody.hpp"
#include <iostream>

void CollisionListener::BeginContact(b2Contact* contactInfo) {
	auto firstBody = (PhysicsBody*)contactInfo->GetFixtureA()->GetBody()->GetUserData().pointer;
	auto first = contactInfo->GetFixtureA();
	auto second = contactInfo->GetFixtureB();

	firstBody->onCollisionStart(first, second);
}

void CollisionListener::EndContact(b2Contact* contactInfo) {
	auto firstBody = (PhysicsBody*)contactInfo->GetFixtureA()->GetBody()->GetUserData().pointer;
	auto first = contactInfo->GetFixtureA();
	auto second = contactInfo->GetFixtureB();

	firstBody->onCollisionEnd(first, second);
}
