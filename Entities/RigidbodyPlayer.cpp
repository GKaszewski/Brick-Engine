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
	setAsRectangle(32, 32);
	createBody();
	
	body->GetUserData().pointer = (uintptr_t)this;
	printf("Created player!\n");
}

RigidbodyPlayer::~RigidbodyPlayer() {
	
}

void RigidbodyPlayer::createBody(float friction) {
	PhysicsBody::createBody(friction);
	body->SetFixedRotation(true);

	b2PolygonShape footShape;
	b2FixtureDef footFixtureDef;
	footShape.SetAsBox(0.3f, 0.3f, b2Vec2(0, 0.3f), 0);
	footFixtureDef.shape = &footShape;
	footFixtureDef.density = 1;
	footFixtureDef.isSensor = true;

	auto fixture = body->CreateFixture(&footFixtureDef);
	fixture->GetUserData().pointer = FOOT_ID;
}

void RigidbodyPlayer::handleEvent(sf::Event e) {
	if (e.type == sf::Event::KeyReleased) {
		desiredVelocity = 0.0f;
	}
}

void RigidbodyPlayer::handleMovement() {

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && hasGround) {
		this->ApplyImpulse(b2Vec2(0.0f, -jumpSpeed));
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		desiredVelocity = -speed;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		desiredVelocity = speed;
	}


	auto currentVelocity = body->GetLinearVelocity();
	auto deltaVelocity = desiredVelocity - currentVelocity.x;
	auto impulse = body->GetMass() * deltaVelocity;
	body->ApplyLinearImpulse(b2Vec2(impulse, 0.0f), body->GetWorldCenter(), true);
}

void RigidbodyPlayer::onCollisionStart(b2Fixture* us, b2Fixture* other) {
	if (us->GetUserData().pointer == FOOT_ID) {
		hasGround = true;
	}
}

void RigidbodyPlayer::onCollisionEnd(b2Fixture* us, b2Fixture* other) {
	if (us->GetUserData().pointer == FOOT_ID) {
		hasGround = false;
	}
}
