#pragma once
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include "PhysicsBody.hpp"
#include "../Physics/CollisionListener.hpp"
#include <iostream>

class RigidbodyPlayer : public PhysicsBody {
public:
	RigidbodyPlayer(const sf::Vector2f& position, float speed = 5.f, float jumpSpeed = 10.0f);
	~RigidbodyPlayer();

	void createBody(float friction = 0.3f) override;

	void handleEvent(sf::Event e);
	void handleMovement();

	void onCollisionStart(b2Fixture* us, b2Fixture* other) override;
	void onCollisionEnd(b2Fixture* us, b2Fixture* other) override;
	
	const int FOOT_ID = 2;
	bool hasGround = false;
private:
	float speed, jumpSpeed;
	float desiredVelocity;
};

