#include "TestState.hpp"
#include "../Game.hpp"
#include <cmath>
#include <iostream>
#include <memory>
#include "../Managers/EntityManager.hpp"

TestState::TestState(Game& game, const char* name) : State(game, name) {
	tilemap = TileMap();
	const int level[] =
	{
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,-1,-1,-1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,-1,-1,-1, -1, -1,
		40, 41, 41, 41, 42, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 40, 41, 41, 41, 42,
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,-1,-1,-1,-1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,-1,-1,-1,-1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,-1,-1,-1,-1, -1,
		-1, -1, -1, -1, -1, -1, -1, 40, 41, 41, 41, 41, 42, -1, -1,-1,-1,-1,-1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,-1,-1,-1,-1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,-1,-1,-1,-1, -1,
		-1, -1, -1, -1, -1, 40, 41, 41, 41, 41, 41, 41, 41, 41, 42,-1,-1,-1,-1, -1,
		40, 41, 41, 41, 41, 42, -1, -1, -1, -1, -1, -1, -1, -1, 40, 41, 41, 41, 41, 42,
	};

	auto tileset = &ResourceHolder::get().textures.get("tileset");
	tilemap.load(tileset, { 16, 16 }, {64, 64}, level, 20, 11);
	tilemap.setPosition({ 0.0f, 0.0f});

	testBody = new Bottle({ 480.0f, 320.0f });
	player = new RigidbodyPlayer({ 580.0f, 320.0f });

	PhysicsManager::getInstance()->world->SetContactListener(&collisionListener);
}

TestState::~TestState() {
}

void TestState::handleInput() {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
		testBody->setPosition({ 480, 230 });
		player->setPosition({ 580, 230 });
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		player->handleMovement(RigidbodyPlayer::Direction::UP);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		player->handleMovement(RigidbodyPlayer::Direction::LEFT);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		player->handleMovement(RigidbodyPlayer::Direction::RIGHT);
	}
}

void TestState::handleEvent(sf::Event e) {
	if (e.type == sf::Event::MouseButtonPressed) {
		if (vertexAdded < 3) {
			sf::Vertex vertex(game->getWindow().mapPixelToCoords(sf::Mouse::getPosition(game->getWindow())), sf::Color(100, 0, 0, 200));
			auto tileSize = tilemap.getTileSize();
			vertex.position = { roundf(vertex.position.x / tileSize.x) * tileSize.x, roundf(vertex.position.y / tileSize.y) * tileSize.y };
			points.push_back(vertex.position);
			vertexAdded++;
		}
		if (vertexAdded == 3) {
			auto collider = new PhysicsBody(nullptr, points[1], PhysicsBody::Type::STATIC);
			collider->setAsTriangle(points);
			collider->createBody();
			vertexAdded = 0;
			points.clear();
		}
	}

	if (e.type == sf::Event::KeyPressed) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::P)) {
			preview = !preview;
		}
	}
}

void TestState::update(sf::Time deltaTime) {
	EntityManager::getInstance()->destroyBodies();
	for (auto entity : EntityManager::getInstance()->physicsEntities) {
		entity->update();
	}
}

void TestState::fixedUpdate(sf::Time deltaTime) {
}

void TestState::render(sf::RenderTarget& renderer) {
	renderer.clear(sf::Color(179, 205, 224));
	renderer.draw(tilemap);
	for (auto entity : EntityManager::getInstance()->physicsEntities) {
		entity->render(renderer);
	}
	/*if (preview) {
		for (auto& element : colliders) {
			element.render(renderer);
		}
	}*/
}
