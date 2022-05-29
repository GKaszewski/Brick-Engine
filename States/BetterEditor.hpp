#pragma once
#include <tmxlite/Map.hpp>
#include <string>
#include <vector>
#include <memory>

#include "../State.hpp"
#include "../Game.hpp"
#include "../Graphics/TmxMap.hpp"
#include "../Graphics/Grid.hpp"
#include "../Tools/ColliderPlacementTool.hpp"
#include "../Entities/RigidbodyPlayer.hpp"

#include <imgui.h>
#include <imgui-SFML.h>

class BetterEditor : public State {
public:
	BetterEditor(Game& game, const char* name);
	~BetterEditor();
	void handleInput() override;
	void handleEvent(sf::Event e) override;
	void update(sf::Time deltaTime) override;
	void fixedUpdate(sf::Time deltaTime) override;
	void render(sf::RenderTarget& renderer) override;
private:
	tmx::Map map;
	std::vector<std::unique_ptr<TmxMapLayer>> mapLayers;
	TileMap tileMapInfo;
	ColliderPlacementTool colliderTool;
	Grid grid;
	RigidbodyPlayer* player;
	CollisionListener collisionListener;

	void drawGUI();
	void loadMap(const std::string& filename);
	void setupGrid();
	void saveColliders();
	void loadColliders();
	void loadColliders(const std::string & filename);

	bool displayColliders = false;
	bool play = true;
	bool displayGrid = false;

	char collidersFileNameSaveBuffer[64];
	char collidersFileNameLoadBuffer[64];
	char mapNameBuffer[64];
};

