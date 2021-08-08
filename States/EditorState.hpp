#pragma once
#include "../State.hpp"
#include "../Entities/Cell.hpp"
#include "../Entities/Player.hpp"
#include "../GUI/Button.hpp"
#include <memory>
#include <imgui.h>
#include <imgui-SFML.h>

class EditorState : public State {
public:
	EditorState(Game & game, const char * name);
	~EditorState();
	void handleInput() override;
	void handleEvent(sf::Event e) override;
	void update(sf::Time deltaTime) override;
	void fixedUpdate(sf::Time deltaTime) override;
	void render(sf::RenderTarget& renderer) override;

private:
	int gridSize = 50;
	std::vector<std::vector<Cell>> grid;
	std::vector<sf::VertexArray> colliders;
	sf::View camera;
	sf::RectangleShape previewTile;
	sf::Texture tileset;
	sf::Texture penumbraTexture;
	int currentTile = 0;
	sf::Color backgroundColor;
	std::unique_ptr<Button> testButton;

	bool continousPainting = false;
	bool mouseLocked = false;

	bool boxSelection = false;
	bool mouseDrag = false;

	bool playMode = false;
	bool placePlayer = false;

	bool collisionsMode = false;

	bool displayHelp = true;

	sf::RectangleShape box;
	sf::Vector2i lastMousePos;

	int collidersAmount = 0;
	int vertexsAdded = 0;
	bool startedAddingCollisions = false;
	sf::IntRect tilePosition;

	std::string currrentState = "";
	int tiles;
	
	float clearColor[4];
	ImVec4 cColor;

	Player* archer = nullptr;

	void drawGUI();
	void createCollider();
	void createBoxSelection();
	void moveCamera(float speed);
	void selectTile();
	void scroll(sf::Event e);
	void clearScene(float* value);
	void fillScene();
};