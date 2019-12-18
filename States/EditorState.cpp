#include "EditorState.hpp"
#include "../Game.hpp"
#include "../ResourceHolder.hpp"
#include "../Entities/Player.hpp"
#include <string>
#include <iostream>

EditorState::EditorState(Game & game, const char * name) : State(game, name) {
	ImGui::SFML::Init(game.getWindow());
	currrentState = "Edit mode";
	ResourceHolder::get().textures.add("tileset");
	grid = std::vector<std::vector<Cell>>(gridSize, std::vector<Cell>(gridSize));
	for (int i = 0; i < gridSize; i++) {
		for (int j = 0; j < gridSize; j++) {
			grid[i][j] = Cell(sf::Vector2f(i, j));
		}
	}
	colliders.push_back(sf::VertexArray());
	colliders[0].setPrimitiveType(sf::TrianglesFan);
	tileset = ResourceHolder::get().textures.get("tileset");
	previewTile.setSize({ 64, 64 });
	sf::IntRect tilePosition(0, 0, 16, 16);
	previewTile.setTexture(&tileset);
	previewTile.setTextureRect(sf::IntRect(0, 0, 16, 16));

	camera.reset(sf::FloatRect(0, 0, 1280, 720));
	camera.setSize({ 1280, 720 });

	backgroundColor = sf::Color::White;
	box.setFillColor(sf::Color(144, 238, 144, 200));

	tilePosition = sf::IntRect(0, 0, 16, 16);
	clearColor[0] = clearColor[1] = clearColor[2] = clearColor[3]=0.0f;

	testButton = makeButton();
	testButton->setText("Test button");
	testButton->setFunction([]() {
		std::cout << "Hello!\n";
	});
	testButton->setPosition({-320, 240});
}

EditorState::~EditorState() {
	ImGui::SFML::Shutdown();
}

void EditorState::handleInput() {
	if (game->getWindow().hasFocus() && !playMode) {

		moveCamera(10);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z)) {
			previewTile.rotate(10);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::X)) {
			previewTile.rotate(-10);
		}

		selectTile();
	}

	if (playMode) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
			archer->move(Player::Direction::UP, 10);
			for (int i = 0; i < colliders.size(); i++) {
				if (archer->body.getGlobalBounds().intersects(colliders[i].getBounds())) {
					archer->move(Player::Direction::DOWN, 10);
				}
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
			archer->move(Player::Direction::DOWN, 10);
			for (int i = 0; i < colliders.size(); i++) {
				if (archer->body.getGlobalBounds().intersects(colliders[i].getBounds())) {
					archer->move(Player::Direction::UP, 10);
				}
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
			archer->move(Player::Direction::LEFT, 10);
			for (int i = 0; i < colliders.size(); i++) {
				if (archer->body.getGlobalBounds().intersects(colliders[i].getBounds())) {
					archer->move(Player::Direction::RIGHT, 10);
				}
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
			archer->move(Player::Direction::RIGHT, 10);
			for (int i = 0; i < colliders.size(); i++) {
				if (archer->body.getGlobalBounds().intersects(colliders[i].getBounds())) {
					archer->move(Player::Direction::LEFT, 10);
				}
			}
		}

	}
}

void EditorState::handleEvent(sf::Event e) {
	ImGuiIO io = ImGui::GetIO();
	ImGui::SFML::ProcessEvent(e);
	testButton->handleEvents(e, game->getWindow());
	if (e.type == sf::Event::KeyPressed) {
		switch (e.key.code) {
		case sf::Keyboard::Num1:
		case sf::Keyboard::Num2:
		case sf::Keyboard::Num3:
		case sf::Keyboard::Num4:
		case sf::Keyboard::Num5:
		case sf::Keyboard::Num6: {
			tilePosition.left += (16 * currentTile);
			if (tilePosition.left >= 96)
				tilePosition.left = 0;
			break;
		}
		case sf::Keyboard::Num0:
			placePlayer = !placePlayer;
			break;
		case sf::Keyboard::P:
			continousPainting = !continousPainting;
			break;
		case sf::Keyboard::F: {
			/*currentState.setString("FILL SCENE");*/
			fillScene();
			break;
		}
		case sf::Keyboard::R: {
			/*currentState.setString("CLEAR SCENE");*/
			clearScene(clearColor);
			break;
		}
		case sf::Keyboard::B: {
			boxSelection = !boxSelection;
			continousPainting = false;
			break;
		}
		case sf::Keyboard::U:
			playMode = !playMode;
			break;
		case sf::Keyboard::C: {
			collisionsMode = !collisionsMode;
			/*if (collisionsMode)
				currentState = "COLLISION MODE";
			else
				currentState = "EDIT MODE";*/
			break;
		}
		case sf::Keyboard::H:
			displayHelp = !displayHelp;
		default:
			break;
		}
	}

	if (e.type == sf::Event::MouseButtonPressed && !playMode && !io.WantCaptureMouse) {
		if (continousPainting) mouseLocked = true;
		else if (collisionsMode) {
			switch (e.mouseButton.button) {
			case sf::Mouse::Left: {
				if (vertexsAdded < 3) {
					sf::Vertex vertex(game->getWindow().mapPixelToCoords(sf::Mouse::getPosition(game->getWindow())), sf::Color(100, 0, 0, 200));
					std::cout << "Vertex pos: " << vertex.position.x << " , " << vertex.position.y << std::endl;
					std::cout << "Vertex: " << vertexsAdded << std::endl;

					colliders[collidersAmount].append(vertex);
					vertexsAdded++;
					std::cout << "Added vertex!" << "(" << vertexsAdded << ")" << std::endl;

				}
				break;
			}
			case sf::Mouse::Right: {
				auto mousePos = game->getWindow().mapPixelToCoords(sf::Mouse::getPosition(game->getWindow()));
				if (colliders.size() > 0) {
					for (int i = 0; i < colliders.size(); i++) {
						if (colliders[i].getBounds().contains(mousePos)) {
							colliders.erase(colliders.begin() + i);
						}
					}
				}
				break;
			}
			}
			if (vertexsAdded == 3) {
				collidersAmount++;
				colliders.push_back(sf::VertexArray());
				colliders[collidersAmount].setPrimitiveType(sf::TrianglesFan);
				std::cout << "Collider added! (" << collidersAmount << ")" << std::endl;
				vertexsAdded = 0;
				collisionsMode = false;
				//currentState.setString("EDIT MODE");
			}
		}
		else if (!continousPainting && !boxSelection && !collisionsMode && !io.WantCaptureMouse) {
			switch (e.mouseButton.button) {
			case sf::Mouse::Left: {
				auto mousePos = sf::Mouse::getPosition(game->getWindow());
				auto translatedPos = game->getWindow().mapPixelToCoords(mousePos);
				for (int i = 0; i < gridSize; i++) {
					for (int j = 0; j < gridSize; j++) {
						if (grid[i][j].checkMouseClick(translatedPos)) {
							tiles++;
							if (placePlayer) {
								archer = new Player(grid[i][j].shape.getPosition());
							}
							else grid[i][j].changeTexture(&tileset, currentTile);
						}
					}
				}
				break;
			}
			case sf::Mouse::Right: {
				auto mousePos = sf::Mouse::getPosition(game->getWindow());
				auto translatedPos = game->getWindow().mapPixelToCoords(mousePos);
				std::cout << "Position: " << translatedPos.x << " , " << translatedPos.y << std::endl;
				for (int i = 0; i < gridSize; i++) {
					for (int j = 0; j < gridSize; j++) {
						if (grid[i][j].checkMouseClick(translatedPos)) {
							grid[i][j].changeColor(sf::Color::White);
							grid[i][j].changeTexture(nullptr);
							tiles--;
						}
					}
				}
				break;
			}
			}
		}
		else if (boxSelection) {
			/*currentState.setString("BOX SELECTION");*/
			mouseDrag = true;
			box.setPosition(game->getWindow().mapPixelToCoords(sf::Mouse::getPosition(game->getWindow())));
		}
	}

	if (e.type == sf::Event::MouseButtonReleased && !playMode) {
		if (continousPainting) mouseLocked = false;
		if (collisionsMode) {

		}
		if (boxSelection) {
			mouseDrag = false;
			box.setSize(game->getWindow().mapPixelToCoords({ e.mouseButton.x, e.mouseButton.y }) - box.getPosition());
			for (int i = 0; i < gridSize; i++) {
				for (int j = 0; j < gridSize; j++) {
					if (grid[i][j].shape.getGlobalBounds().intersects(box.getGlobalBounds())) {
						grid[i][j].changeTexture(&tileset, currentTile);
						tiles++;
					}
				}
			}
			boxSelection = false;
		}

		box.setSize({ 0,0 });
		box.setPosition({ 0,0 });
	}


	if (e.type == sf::Event::MouseMoved && !playMode) {
		if (boxSelection) {
			if (mouseDrag) {
				box.setSize(game->getWindow().mapPixelToCoords({ e.mouseMove.x, e.mouseMove.y }) - box.getPosition());
			}
		}
	}

	if (continousPainting) {
		/*currentState.setString("PAITING MODE");*/
		if (mouseLocked) {
			if (lastMousePos != sf::Mouse::getPosition()) {
				auto mousePos = sf::Mouse::getPosition(game->getWindow());
				auto translatedPos = game->getWindow().mapPixelToCoords(mousePos);
				std::cout << "Position: " << translatedPos.x << " , " << translatedPos.y << std::endl;
				for (int i = 0; i < gridSize; i++) {
					for (int j = 0; j < gridSize; j++) {
						if (grid[i][j].checkMouseClick(translatedPos)) {
							grid[i][j].changeTexture(&tileset, currentTile);
							tiles++;
						}
					}
				}
				lastMousePos = sf::Mouse::getPosition();
			}
		}
	}

	scroll(e);
}

void EditorState::update(sf::Time deltaTime) {
	float fps = 1.0f / deltaTime.asSeconds();
	game->getWindow().setTitle("Level editor FPS: " + std::to_string(fps));

	if (playMode) {
		camera.setCenter(archer->body.getPosition());
	}
	
	ImGui::SFML::Update(game->getWindow(), deltaTime);
	
	drawGUI();
	
	auto previewTilePos = game->getWindow().mapPixelToCoords(sf::Mouse::getPosition(game->getWindow()));
	previewTile.setPosition(((int)(previewTilePos.x / previewTile.getSize().x)) * 64, ((int)(previewTilePos.y / previewTile.getSize().y)) * 64);
	previewTile.setFillColor(sf::Color(255, 255, 255, 100));
}

void EditorState::fixedUpdate(sf::Time deltaTime) {

}

void EditorState::render(sf::RenderTarget& renderer) {
	renderer.setView(camera);
	renderer.clear(backgroundColor);


	for (int i = 0; i < gridSize; i++) {
		for (int j = 0; j < gridSize; j++) {
			renderer.draw(grid[i][j].shape);
		}
	}

	if (boxSelection) {
		renderer.draw(box);
	}
	else {
		box.setPosition({ 0,0 });
		box.setSize({ 0,0 });
		if (!playMode) {
			renderer.draw(previewTile);
			for (auto& element : colliders) {
				renderer.draw(element);
			}
		}
	}

	testButton->render(renderer);


	if (archer != nullptr)
		renderer.draw(archer->body);

	ImGui::SFML::Render(renderer);
}

void EditorState::drawGUI() {
	ImGui::Begin("Info [Debug]", nullptr, ImGuiWindowFlags_MenuBar);
	ImGui::Text(currrentState.c_str());
	ImGui::Text("Tiles: %d", tiles);
	if (ImGui::BeginMenuBar()) {
		if (ImGui::BeginMenu("File...")) {
			if (ImGui::Button("New...")) {
				clearScene(clearColor);
			}
			ImGui::Button("Load scene...");
			ImGui::Button("Save scene...");
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit...")) {
			ImGui::ColorEdit4("Set clear scene color", clearColor);
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
	
	ImGui::End();

	ImGui::Text("Color: %f, %f, %f, %f", *clearColor, clearColor[1], clearColor[2], clearColor[3]);
	
	if(archer != nullptr)
		ImGui::Text("PlayMode: %d, Player X: %f, Y: %f", playMode, archer->body.getPosition().x, archer->body.getPosition().y);

	if (displayHelp) {
		ImGui::Begin("Help");
		ImGui::Text("Move: WSAD");
		ImGui::Text("Zoom: Mouse Scroll");
		ImGui::Text("Modes: P - painting, C - collision, U - play, B - box, F - fill, R - clear");
		ImGui::Text("H - this box");
		ImGui::Text("1-6 place tiles");
		ImGui::Text("0 place player");
		ImGui::End();
	}
}

void EditorState::createCollider() {

}

void EditorState::createBoxSelection() {

}

void EditorState::moveCamera(float speed) {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
		camera.move(0, -speed);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
		camera.move(0, speed);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
		camera.move(-speed, 0);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
		camera.move(speed, 0);
	}
}

void EditorState::selectTile() {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num1)) {
		currentTile = 0;
		previewTile.setTextureRect(sf::IntRect(0, 0, 16, 16));
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num2)) {
		currentTile = 1;
		previewTile.setTextureRect(sf::IntRect(16, 0, 16, 16));
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num3)) {
		currentTile = 2;
		previewTile.setTextureRect(sf::IntRect(32, 0, 16, 16));
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num4)) {
		currentTile = 3;
		previewTile.setTextureRect(sf::IntRect(48, 0, 16, 16));
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num5)) {
		currentTile = 4;
		previewTile.setTextureRect(sf::IntRect(64, 0, 16, 16));
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num6)) {
		currentTile = 5;
		previewTile.setTextureRect(sf::IntRect(80, 0, 16, 16));
	}
}

void EditorState::scroll(sf::Event e) {
	if (e.type == sf::Event::MouseWheelScrolled) {
		if (e.mouseWheelScroll.delta > 0)
			camera.zoom(1.f / 1.1f);
		else if (e.mouseWheelScroll.delta < 0)
			camera.zoom(1.1f);
	}
}

void EditorState::clearScene(float* value) {
	int howManyCleaned = 0;
	for (int i = 0; i < gridSize; i++) {
		for (int j = 0; j < gridSize; j++) {
			backgroundColor = sf::Color(*value * 255, value[1] * 255, value[2] * 255, value[3] * 255);
			grid[i][j].changeColor(sf::Color(*value * 255, value[1] * 255, value[2] * 255, value[3] * 255));
			if (grid[i][j].hasTexture) {
				backgroundColor = sf::Color(*value * 255, value[1] * 255, value[2] * 255, value[3] * 255);
				grid[i][j].changeTexture(nullptr);
				grid[i][j].changeColor(sf::Color(*value*255, value[1] * 255, value[2] * 255, value[3] * 255));
				howManyCleaned++;
				tiles = 0;
			}
		}
	}
	currentTile = 0;
	std::cout << "Cleaned " << howManyCleaned << " tiles!\n";
}

void EditorState::fillScene() {
	currentTile = 2;
	int howManyFilled = 0;
	for (int i = 0; i < gridSize; i++) {
		for (int j = 0; j < gridSize; j++) {
			if (!grid[i][j].hasTexture) {
				backgroundColor = sf::Color(52, 152, 219, 255);
				grid[i][j].changeTexture(&tileset, currentTile);
				howManyFilled++;
				tiles = howManyFilled;
			}
		}
	}
	currentTile = 0;
	std::cout << "Filled " << howManyFilled << " tiles!\n";
}
