#include "TestState.hpp"
#include "../Core/Game.hpp"
#include <cmath>
#include <sstream>
#include <iostream>

TestState::TestState(Game& game, const char* name) : State(game, name), colliderTool(game, {64, 64}), boxPaintingTool(game, 4, grid) {
	testBody = PhysicsBody("bottle", {480, 230});
	ground = PhysicsBody("ground", { 0, 0 }, PhysicsBody::Type::STATIC);
	testBody.setAsRectangle(16, 16);
	ground.setAsRectangle(64, 64);
	testBody.createBody();
	ground.createBody();
	tilemap = TileMap();
	grid = Grid();
	map.load("assets/demo.tmx");
	layerZero = std::make_unique<TmxMapLayer>(map, 0);
	layerOne = std::make_unique<TmxMapLayer>(map, 1);
	layerTwo = std::make_unique<TmxMapLayer>(map, 2);
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
}

TestState::~TestState() {
}

void TestState::handleInput() {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
		testBody.setPosition({ 480, 230 });
	}
}

void TestState::handleEvent(sf::Event e) {
	if (e.type == sf::Event::MouseButtonPressed) {
		colliderTool.placeCollider();
	}

	if (e.type == sf::Event::KeyPressed) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::P)) {
			preview = !preview;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LBracket)) {
			colliderTool.saveCollidersToFile("colliders.txt");
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::L)) {
			colliderTool.Colliders() = colliderTool.loadCollidersFromFile("colliders.txt");
		}
	}
}

void TestState::update(sf::Time deltaTime) {
	layerZero->update(deltaTime);
	float fps = 1.0f / deltaTime.asSeconds();
	fps *= 2.0f;
	game->getWindow()->getWindow().setTitle("Test State - (PHYSICS) FPS: " + std::to_string(fps));
	testBody.update();
	ground.update();
}

void TestState::fixedUpdate(sf::Time deltaTime) {
}

void TestState::render(sf::RenderTarget& renderer) {
	renderer.clear(sf::Color(179, 205, 224));
	renderer.draw(grid);
	testBody.render(renderer);
	
	renderer.draw(*layerZero);
	renderer.draw(*layerOne);
	renderer.draw(*layerTwo);

	if (preview) {
		for (auto& element : colliderTool.Colliders()) {
			element.render(renderer);
		}
	}
}
