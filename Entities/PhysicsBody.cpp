#include "PhysicsBody.hpp"
#include "../utils.hpp"
#include "../Managers/EntityManager.hpp"	


PhysicsBody::PhysicsBody(const char* textureName, sf::Vector2f position, Type bodyType, PhysicsManager::TAGS tag, sf::IntRect textureOffset) {
	type = bodyType;
	this->textureOffset = textureOffset;
	if (texture != nullptr) {
		texture = &ResourceHolder::get().textures.get(textureName);
	}
	else {
		texture = &ResourceHolder::get().textures.get("tileset");
		this->textureOffset = sf::IntRect(96, 16, 16, 16);
	}
	this->tag = tag;
	this->position = position;
	if (this->textureOffset.width == 0) {
		this->textureOffset = sf::IntRect(0,0, texture->getSize().x, texture->getSize().y);
	}
}

PhysicsBody::~PhysicsBody() {
	printf("i am deleted\n");
}

void PhysicsBody::setAsTriangle(const std::vector<sf::Vector2f>& points) {
	sf::ConvexShape* triangle = new sf::ConvexShape(3);
	triangle->setPointCount(3);
	triangle->setPoint(0, points[0] - position);
	triangle->setPoint(1, points[1] - position);
	triangle->setPoint(2, points[2] - position);
	sf::Vector2f offset = sf::Vector2f(triangle->getGlobalBounds().width / 2, triangle->getGlobalBounds().height / 2);
	triangle->setOrigin(offset);
	triangle->setPosition(position + offset);
	triangle->setTexture(texture);
	triangle->setTextureRect(textureOffset);
	shape = triangle;
}

void PhysicsBody::setAsRectangle(float width, float height) {
	sf::RectangleShape* rectangle = new sf::RectangleShape(sf::Vector2f(width, height));
	rectangle->setPosition(position);
	rectangle->setOrigin(width / 2, height / 2);
	rectangle->setTexture(texture);
	rectangle->setTextureRect(textureOffset);
	shape = rectangle;

}

void PhysicsBody::setAsCircle(float radius) {
	sf::CircleShape* circle = new sf::CircleShape(radius);
	circle->setPosition(position);
	circle->setOrigin(radius, radius);
	circle->setTexture(texture);
	circle->setTextureRect(textureOffset);
	shape = circle;
}


void PhysicsBody::render(sf::RenderTarget & renderer) {
	renderer.draw(*shape);
}

void PhysicsBody::update() {
	if (body == nullptr) return;
	shape->setPosition(utils::convert_b2vec_to_sfml_vec(body->GetPosition()));
	shape->setRotation(body->GetAngle() * 180.0f / b2_pi);
}

void PhysicsBody::setVelocity(sf::Vector2f movement) {
	body->SetLinearVelocity(utils::convert_sfml_vec_to_b2vec(movement));
}

void PhysicsBody::setVelocity(b2Vec2 velocity) {
	body->SetLinearVelocity(velocity);
}

void PhysicsBody::setPosition(sf::Vector2f position) {
	body->SetTransform(utils::convert_sfml_vec_to_b2vec(position), body->GetAngle());
	body->SetLinearVelocity({ 0.f, 0.0f });
}

void PhysicsBody::ApplyImpulse(b2Vec2 impulse) {
	auto velocity = body->GetLinearVelocity();
	b2Vec2 velocityChange = impulse - velocity;
	
	body->ApplyLinearImpulse({velocityChange.x * body->GetMass(), velocityChange.y * body->GetMass()}, body->GetWorldCenter(), true);
}

void PhysicsBody::ApplyImpulse(sf::Vector2f impulse) {
	auto velocity = body->GetLinearVelocity();
	auto convertedImpulse = utils::convert_sfml_vec_to_b2vec(impulse);
	b2Vec2 velocityChange = convertedImpulse - velocity;
	
	body->ApplyLinearImpulse({velocityChange.x * body->GetMass(), velocityChange.y * body->GetMass() }, body->GetWorldCenter(), true);
}

void PhysicsBody::AddForce(b2Vec2 force) {
	body->ApplyForce(force, body->GetWorldCenter(), true);
}

void PhysicsBody::AddForce(sf::Vector2f force) {
	body->ApplyLinearImpulse(utils::convert_sfml_vec_to_b2vec(force), body->GetWorldCenter(), true);
}

void PhysicsBody::onCollisionStart(b2Fixture* us, b2Fixture* other) {}

void PhysicsBody::onCollisionEnd(b2Fixture* us, b2Fixture* other) {}

void PhysicsBody::createBody(float friction) {
	switch (type) {
	case PhysicsBody::Type::STATIC:
		bodyDef.type = b2_staticBody;
		break;
	case PhysicsBody::Type::DYNAMIC:
		bodyDef.type = b2_dynamicBody;
		break;
	case PhysicsBody::Type::KINEMATIC:
		bodyDef.type = b2_kinematicBody;
		break;
	}
	auto vec = utils::convert_sfml_vec_to_b2vec(this->shape->getPosition());
	bodyDef.position.Set(vec.x, vec.y);
	body = PhysicsManager::getInstance()->world->CreateBody(&bodyDef);
	
	b2PolygonShape shape;
	shape.SetAsBox((this->shape->getGlobalBounds().width / 2) / PhysicsManager::SCALE, (this->shape->getGlobalBounds().height / 2) / PhysicsManager::SCALE);
	b2FixtureDef fixtureDef;
	switch (type) {
	case PhysicsBody::Type::STATIC:
		fixtureDef.density = 0.f;
		break;
	case PhysicsBody::Type::DYNAMIC:
		fixtureDef.density = 1.f;
		break;
	case PhysicsBody::Type::KINEMATIC:
		fixtureDef.density = 1.f;
	}

	fixtureDef.shape = &shape;
	fixtureDef.friction = friction;
	body->CreateFixture(&fixtureDef);

	body->GetUserData().pointer = (uintptr_t)this;
	EntityManager::getInstance()->physicsEntities.push_back(this);
	printf("Added new entity.\n");
}
