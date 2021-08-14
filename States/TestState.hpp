#pragma once
#include "../Core/State.hpp"
#include "../Entities/PhysicsBody.hpp"
#include "../Graphics/TileMap.hpp"
#include "../Graphics/Grid.hpp"
#include "../Graphics/TmxMap.hpp"
#include "../Tools/ColliderPlacementTool.hpp"
#include "../Tools/BoxPaintingTool.hpp"
#include <vector>
#include <memory>
#include <tmxlite/Map.hpp>

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
	TileMap tilemap;
	Grid grid;
	ColliderPlacementTool colliderTool;
	BoxPaintingTool boxPaintingTool;
	SelectionData selectionData;
	std::vector<sf::RectangleShape> test;
	bool preview = false;

	tmx::Map map;
	std::unique_ptr<TmxMapLayer> layerZero;
	std::unique_ptr<TmxMapLayer> layerOne;
	std::unique_ptr<TmxMapLayer> layerTwo;
};

