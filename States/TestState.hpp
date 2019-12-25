#pragma once
#include "../State.hpp"
#include "../Entities/PhysicsBody.hpp"
class TestState : public State {
public:
	TestState(Game& game, const char* name);
	~TestState();
	void handleInput() override;
	void handleEvent(sf::Event e) override;
	void update(sf::Time deltaTime) override;
	void fixedUpdate(sf::Time deltaTime) override;
	void render(sf::RenderTarget& renderer) override;

private:
	PhysicsBody testBody;
	PhysicsBody ground;
};

