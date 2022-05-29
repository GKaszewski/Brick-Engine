#include "BetterEditor.hpp"
#include <iostream>

BetterEditor::BetterEditor(Game& game, const char* name) : State(game, name), colliderTool(game, { 64, 64 }) {
	ImGui::SFML::Init(game.getWindow());
	grid = Grid(game.getWindow().getSize(), { 32, 32 }, { 0.0f, 0.0f });
	player = new RigidbodyPlayer({ 320.f, 0.0f }, 10.f);

	loadMap("assets/map2.tmx");
	loadColliders("colliders_data.txt");
	PhysicsManager::getInstance()->world->SetContactListener(&collisionListener);
}

BetterEditor::~BetterEditor() {
	ImGui::SFML::Shutdown();
}

void BetterEditor::handleInput() {
}

void BetterEditor::handleEvent(sf::Event e) {
	ImGuiIO io = ImGui::GetIO();
	ImGui::SFML::ProcessEvent(e);

	if (e.type == sf::Event::MouseButtonPressed && !io.WantCaptureMouse) {
		colliderTool.placeCollider();
	}

	if (play) player->handleEvent(e);
}

void BetterEditor::update(sf::Time deltaTime) {
	ImGui::SFML::Update(game->getWindow(), deltaTime);
	drawGUI();
	if (play) {
		player->handleMovement();
	}
	player->update();
}

void BetterEditor::fixedUpdate(sf::Time deltaTime) {
}

void BetterEditor::render(sf::RenderTarget& renderer) {
	for (auto& layer : mapLayers) {
		renderer.draw(*layer);
	}
	
	player->render(renderer);

	if (displayGrid) {
		renderer.draw(grid);
	}

	if (displayColliders) {
		for (auto& element : colliderTool.Colliders()) {
			element->render(renderer);
		}
	}

	ImGui::SFML::Render(renderer);
}

void BetterEditor::drawGUI() {
	ImGui::Begin("Info [Debug]", nullptr, ImGuiWindowFlags_MenuBar);
	if (ImGui::BeginMenuBar()) {
		if (ImGui::BeginMenu("File...")) {
			if (ImGui::Button("Load tiled map...")) {
				loadMap(std::string(mapNameBuffer));
			}
			if (ImGui::Button("Save colliders...")) {
				saveColliders();
			}
			if (ImGui::Button("Load colliders...")) {
				loadColliders();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
	ImGui::Checkbox("Grid", &displayGrid);
	ImGui::Checkbox("Colliders preview", &displayColliders);
	ImGui::Checkbox("Play", &play);
	ImGui::InputText("Map", mapNameBuffer, IM_ARRAYSIZE(mapNameBuffer));
	ImGui::InputText("Colliders save", collidersFileNameSaveBuffer, IM_ARRAYSIZE(collidersFileNameSaveBuffer));
	ImGui::InputText("Colliders load", collidersFileNameLoadBuffer, IM_ARRAYSIZE(collidersFileNameLoadBuffer));
	ImGui::End();
}

void BetterEditor::loadMap(const std::string& filename) {
	map.load(filename);
	int layerCount = map.getLayers().size();
	for (int i = 0; i < layerCount; ++i) {
		mapLayers.push_back(std::make_unique<TmxMapLayer>(map, i));
	}

	setupGrid();
}

void BetterEditor::setupGrid() {
	sf::Vector2u size = { map.getTileSize().x, map.getTileSize().y };
	grid = Grid(game->getWindow().getSize(), size, { 0.0f, 0.0f });
	colliderTool.TileSize() = size;
}

void BetterEditor::saveColliders() {
	colliderTool.saveCollidersToFile(std::string(collidersFileNameSaveBuffer));
}

void BetterEditor::loadColliders() {
	colliderTool.Colliders() = colliderTool.loadCollidersFromFile(std::string(collidersFileNameLoadBuffer));
}

void BetterEditor::loadColliders(const std::string& filename) {
	colliderTool.Colliders() = colliderTool.loadCollidersFromFile(filename);
}
