#include "Bottle.hpp"
#include <iostream>
#include "../Managers/EntityManager.hpp"

Bottle::Bottle(sf::Vector2f position) {
	type = Type::DYNAMIC;
	ResourceHolder::get().textures.add("bottle");
	this->position = position;
	texture = &ResourceHolder::get().textures.get("bottle");
	this->position = position;
	if (textureOffset.width == 0) {
		textureOffset = sf::IntRect(0, 0, texture->getSize().x, texture->getSize().y);
	}
	setAsRectangle(16, 16);
	createBody();
	body->GetUserData().pointer = (uintptr_t)this;
}

Bottle::~Bottle() {
	printf("---- from bottle ----\n");
}

void Bottle::onCollisionStart(PhysicsBody* other) {
	printf("Other tag: %i\n", other->Tag());
	if (other->Tag() == PhysicsManager::TAGS::PLAYER) {
		printf("this is from the bottle callback!\n");
		EntityManager::getInstance()->physicsBodiesToRemove.insert(this);
	}
}
