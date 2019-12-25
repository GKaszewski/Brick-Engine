#include "TestState.hpp"
#include "../Game.hpp"

TestState::TestState(Game& game, const char* name) : State(game, name) {
	testBody = PhysicsBody("bottle", {480, 230});
	ground = PhysicsBody("ground", { 480, 330 }, PhysicsBody::Type::STATIC);
}

TestState::~TestState() {
}

void TestState::handleInput() {
}

void TestState::handleEvent(sf::Event e) {
}

void TestState::update(sf::Time deltaTime) {
}

void TestState::fixedUpdate(sf::Time deltaTime) {
}

void TestState::render(sf::RenderTarget& renderer) {
	testBody.render(renderer);
	ground.render(renderer);
}
