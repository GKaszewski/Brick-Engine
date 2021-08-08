#pragma once
#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include "../Managers/PhysicsManager.hpp"
#include "../ResourceHolder.hpp"

class PhysicsBody {
public:
	enum class Type
	{
		STATIC,
		DYNAMIC,
		KINEMATIC
	};
	PhysicsBody() = default;
	PhysicsBody(const char* textureName, sf::Vector2f position = { 0.0f, 0.0f }, Type bodyType = Type::DYNAMIC, int tag = -1, sf::IntRect textureOffset = {});
	~PhysicsBody();
	void setAsTriangle(const std::vector<sf::Vector2f> & points);
	void setAsRectangle(float width, float height);
	void setAsCircle(float radius);
	void createBody(float friction = 0.3f);

	void render(sf::RenderTarget & renderer);
	void update();
	void setVelocity(sf::Vector2f movement);
	void setVelocity(b2Vec2 velocity);
	void setPosition(sf::Vector2f position);

	void ApplyImpulse(b2Vec2 impulse);
	void ApplyImpulse(sf::Vector2f impulse);
	void AddForce(b2Vec2 force);
	void AddForce(sf::Vector2f force);
	
	sf::Sprite& Sprite() { return graphicsBody; }
	b2Body& Body() { return *body; }
	sf::IntRect& TextureOffset() { return textureOffset; }
private:
	sf::Sprite graphicsBody;
	sf::Shape* shape;
	sf::Vector2f position;
	sf::IntRect textureOffset;
	const sf::Texture* texture;
	b2BodyDef bodyDef;
	b2Body* body;
	Type type;
	int tag;
};