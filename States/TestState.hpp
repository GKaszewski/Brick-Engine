#pragma once
#include "../State.hpp"
#include "../Entities/PhysicsBody.hpp"
#include "../Graphics/TileMap.hpp"
#include "../Entities/RigidbodyPlayer.hpp"
#include "../Physics/CollisionListener.hpp"
#include "../Entities/Bottle.hpp"
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
	Bottle* testBody;
	RigidbodyPlayer* player;
	CollisionListener collisionListener;
	TileMap tilemap;
	std::vector<PhysicsBody> colliders;
	std::vector<sf::Vector2f> points;
	int vertexAdded = 0;
	int collidersAmount = 0;
	bool preview = false;
};

