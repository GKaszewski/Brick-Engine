#pragma once
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include "../Managers/PhysicsManager.hpp"
#include "../ResourceHolder.hpp"

class PhysicsBody {
public:
	enum class Type
	{
		STATIC,
		DYNAMIC
	};
	PhysicsBody() = default;
	PhysicsBody(const char* textureName, sf::Vector2f position = {0.0f, 0.0f}, Type bodyType = Type::DYNAMIC);
	void render(sf::RenderTarget & renderer);
	void move(sf::Vector2f movement);
	void setPosition(sf::Vector2f position);
private:
	sf::Sprite graphicsBody;
	b2BodyDef bodyDef;
	b2Body* body;
	Type type;
};