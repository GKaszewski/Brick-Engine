#pragma once
#include "PhysicsBody.hpp"
#include <SFML/Graphics.hpp>
class Bottle : public PhysicsBody {
public:
	Bottle(sf::Vector2f position = { 0.0f, 0.0f });
	~Bottle();
	void onCollisionStart(b2Fixture* us, b2Fixture* other) override;
private:
};

