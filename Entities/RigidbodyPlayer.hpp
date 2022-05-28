#pragma once
#include <SFML/Graphics.hpp>
#include "PhysicsBody.hpp"

class RigidbodyPlayer : public PhysicsBody {
public:
	RigidbodyPlayer(const sf::Vector2f& position, float speed = 10.f, float jumpSpeed = 20.0f);
	~RigidbodyPlayer();

	enum class Direction { UP, DOWN, RIGHT, LEFT };

	void handleMovement(Direction direction);
	void onCollisionStart(PhysicsBody* other) override;
private:
	float speed, jumpSpeed;
};

