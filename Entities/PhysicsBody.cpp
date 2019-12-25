#include "PhysicsBody.hpp"


PhysicsBody::PhysicsBody(const char* textureName, sf::Vector2f position, Type bodyType) {
	type = bodyType;
	graphicsBody.setTexture(ResourceHolder::get().textures.get(textureName));
	graphicsBody.setPosition(position);

	bodyDef.position = b2Vec2(graphicsBody.getGlobalBounds().left / PhysicsManager::SCALE, graphicsBody.getGlobalBounds().top / PhysicsManager::SCALE);
	switch (type) {
	case PhysicsBody::Type::STATIC:
		bodyDef.type = b2_staticBody;
		break;
	case PhysicsBody::Type::DYNAMIC:
		bodyDef.type = b2_dynamicBody;
		break;
	}

	body = PhysicsManager::getInstance()->world->CreateBody(&bodyDef);

	b2PolygonShape Shape;
	Shape.SetAsBox((graphicsBody.getGlobalBounds().width / 2) / PhysicsManager::SCALE, (graphicsBody.getGlobalBounds().height / 2) / PhysicsManager::SCALE);
	b2FixtureDef FixtureDef;
	switch (type) {
	case PhysicsBody::Type::STATIC:
		FixtureDef.density = 0.f;
		break;
	case PhysicsBody::Type::DYNAMIC:
		FixtureDef.density = 1.f;
		break;
	}

	FixtureDef.shape = &Shape;
	body->CreateFixture(&FixtureDef);

}


void PhysicsBody::render(sf::RenderTarget & renderer) {
	
	if (body != nullptr) {

		b2Vec2 position = body->GetPosition();
		float  angle = body->GetAngle();

		graphicsBody.setOrigin(graphicsBody.getGlobalBounds().width / 2, graphicsBody.getGlobalBounds().height / 2);
		graphicsBody.setPosition(position.x * PhysicsManager::SCALE, position.y * PhysicsManager::SCALE);
		graphicsBody.setRotation(angle * 180 / b2_pi);
	}

	renderer.draw(graphicsBody);
}