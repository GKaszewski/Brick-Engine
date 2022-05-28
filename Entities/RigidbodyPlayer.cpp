#include "RigidbodyPlayer.hpp"
#include "../Managers/EntityManager.hpp"	

RigidbodyPlayer::RigidbodyPlayer(const sf::Vector2f& position, float speed, float jumpSpeed) {
	this->type = Type::DYNAMIC;
	ResourceHolder::get().textures.add("player");
	this->position = position;
	this->speed = speed;
	this->jumpSpeed = jumpSpeed;
	this->tag = PhysicsManager::TAGS::PLAYER;
	texture = &ResourceHolder::get().textures.get("player");
	if (textureOffset.width == 0) {
		textureOffset = sf::IntRect(0, 0, texture->getSize().x, texture->getSize().y);
	}
	this->setAsRectangle(32, 32);
	this->createBody();
	body->SetFixedRotation(true);
	body->GetUserData().pointer = (uintptr_t)this;
	printf("Created player!\n");
}

RigidbodyPlayer::~RigidbodyPlayer() {
}

void RigidbodyPlayer::handleMovement(Direction direction) {
	switch (direction)
	{
	case RigidbodyPlayer::Direction::UP:
		this->ApplyImpulse(sf::Vector2f(0.0f, -jumpSpeed));
		break;
	case RigidbodyPlayer::Direction::RIGHT:
		this->ApplyImpulse(sf::Vector2f(speed, 0.0f));
		break;
	case RigidbodyPlayer::Direction::LEFT:
		this->ApplyImpulse(sf::Vector2f(-speed, 0.0f));
		break;
	}
}

void RigidbodyPlayer::onCollisionStart(PhysicsBody* other) {
	
}
