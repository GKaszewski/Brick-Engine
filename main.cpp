/*Copyright (C) 2019 Gabriel Kaszewski - All Rights Reserved
* You may use, distribute and modify this code under the 
* terms of the XYZ license, which unfortunately won't be
* written for another century. Hehe
*/

#include "Game.hpp"
#include "States/EditorState.hpp"

//int main() {
//	int gridSize = 50;
//	std::vector<std::vector<Cell>> grid(gridSize, std::vector<Cell>(gridSize));
//	for (int i = 0; i < gridSize; i++) {
//		for (int j = 0; j < gridSize; j++) {
//			grid[i][j] = Cell(sf::Vector2f(i, j));
//		}
//	}
//
//	std::vector<sf::VertexArray> colliders;
//	colliders.push_back(sf::VertexArray());
//	colliders[0].setPrimitiveType(sf::TrianglesFan);
//
//	sf::Texture tileset;
//	tileset.loadFromFile("assets/textures/tileset.png");
//
//	sf::Texture * groundTexture = new sf::Texture;
//	sf::Texture * boxTexture = new sf::Texture;
//
//	groundTexture->loadFromFile("assets/textures/ground.png");
//	boxTexture->loadFromFile("assets/textures/bottle.png");
//
//	sf::RectangleShape previewTile;
//	previewTile.setSize({64, 64});
//	sf::IntRect tilePosition(0, 0, 16, 16);
//	previewTile.setTexture(&tileset);
//	previewTile.setTextureRect(sf::IntRect(0, 0, 16, 16));
//
//	int currentTile = 0;
//
//	sf::ContextSettings settings;
//	settings.antialiasingLevel = 16;
//	sf::RenderWindow window(sf::VideoMode(1280, 720), "Level editor", sf::Style::Default, settings);
//	window.setFramerateLimit(60);
//	window.setVerticalSyncEnabled(true);
//	ImGui::SFML::Init(window);
//
//	sf::View camera(sf::FloatRect(0, 0, 1280, 720));
//	camera.setSize({ 1280, 720 });
//
//	sf::Color backgroundColor = sf::Color::White;
//
//	bool continousPainting = false;
//	bool mouseLocked = false;
//
//	bool boxSelection = false;
//	bool mouseDrag = false;
//
//	bool playMode = false;
//	bool placePlayer = false;
//
//	bool collisionsMode = false;
//
//	sf::Vector2i lastMousePos;
//
//	Player * archer = nullptr;
//
//	sf::RectangleShape box;
//	box.setFillColor(sf::Color(144, 238, 144, 200));
//
//	sf::Clock clock;
//
//	sf::Font font;
//	font.loadFromFile("assets/fonts/roboto.ttf");
//	sf::Text currentState;
//	currentState.setFont(font);
//	currentState.setCharacterSize(48);
//	currentState.setFillColor(sf::Color::White);
//	currentState.setOutlineColor(sf::Color::Black);
//	currentState.setOutlineThickness(2);
//	currentState.setString("EDIT MODE");
//
//	int collidersAmount = 0;
//	int vertexsAdded = 0;
//	bool startedAddingCollisions = false;
//
//	while (window.isOpen()) {
//		sf::Time deltaTime = clock.restart();
//		float fps = 1.0f / deltaTime.asSeconds();
//		window.setTitle("Level editor FPS: " + std::to_string(fps));
//		window.setView(camera);
//		sf::Event event;
//		while (window.pollEvent(event)) {
//			ImGui::SFML::ProcessEvent(event);
//			if (event.type == sf::Event::Closed)
//				window.close();
//
//			if (event.type == sf::Event::KeyPressed) {
//				switch (event.key.code)	{
//				case sf::Keyboard::Num1:
//				case sf::Keyboard::Num2:
//				case sf::Keyboard::Num3:
//				case sf::Keyboard::Num4:
//				case sf::Keyboard::Num5:
//				case sf::Keyboard::Num6: {
//					tilePosition.left += (16 * currentTile);
//					if (tilePosition.left >= 96)
//						tilePosition.left = 0;
//					break;
//				}
//				case sf::Keyboard::Num0:
//					placePlayer = !placePlayer;
//					break;
//				case sf::Keyboard::P:
//					continousPainting = !continousPainting;
//					break;
//				case sf::Keyboard::F: {
//					currentState.setString("FILL SCENE");
//					currentTile = 2;
//					int howManyFilled = 0;
//					for (int i = 0; i < gridSize; i++) {
//						for (int j = 0; j < gridSize; j++) {
//							if (!grid[i][j].hasTexture) {
//								backgroundColor = sf::Color(52, 152, 219, 255);
//								grid[i][j].changeTexture(&tileset, currentTile);
//								howManyFilled++;
//							}
//						}
//					}
//					std::cout << "Filled " << howManyFilled << " tiles!\n";
//					break;
//				}
//				case sf::Keyboard::R: {
//					currentState.setString("CLEAR SCENE");
//					int howManyCleaned = 0;
//					for (int i = 0; i < gridSize; i++) {
//						for (int j = 0; j < gridSize; j++) {
//							if (grid[i][j].hasTexture) {
//								backgroundColor = sf::Color::White;
//								grid[i][j].changeTexture(nullptr);
//								grid[i][j].changeColor(sf::Color::White);
//								howManyCleaned++;
//							}
//						}
//					}
//					currentState.setString("CLEANED " + howManyCleaned);
//					std::cout << "Cleaned " << howManyCleaned << " tiles!\n";
//					break;
//				}
//				case sf::Keyboard::B: {
//					boxSelection = !boxSelection;
//					continousPainting = false;
//					break; 
//				}
//				case sf::Keyboard::U:
//					playMode = !playMode;
//					break;
//				case sf::Keyboard::C: {
//					collisionsMode = !collisionsMode;
//					if (collisionsMode)
//						currentState.setString("COLLISION MODE");
//					else
//						currentState.setString("EDIT MODE");
//				}
//				default:
//					break;
//				}
//			}
//
//			if (event.type == sf::Event::MouseButtonPressed && !playMode) {
//				if (continousPainting) mouseLocked = true;
//				else if (collisionsMode) {
//					switch (event.mouseButton.button) {
//					case sf::Mouse::Left: {
//						if (vertexsAdded < 3) {
//							sf::Vertex vertex(window.mapPixelToCoords(sf::Mouse::getPosition(window)), sf::Color(100, 0, 0, 200));
//							std::cout << "Vertex pos: " << vertex.position.x << " , " << vertex.position.y << std::endl;
//							std::cout << "Vertex: " << vertexsAdded << std::endl;
//						
//							colliders[collidersAmount].append(vertex);
//							vertexsAdded++;
//							std::cout << "Added vertex!" << "(" << vertexsAdded << ")" << std::endl;
//							
//						}
//						break;
//					}
//					case sf::Mouse::Right: {
//						break;
//					}
//						
//					}
//					if(vertexsAdded == 3) {
//						collidersAmount++;
//						colliders.push_back(sf::VertexArray());
//						colliders[collidersAmount].setPrimitiveType(sf::TrianglesFan);
//						std::cout << "Collider added! (" << collidersAmount << ")" << std::endl;
//						vertexsAdded = 0;
//						collisionsMode = false;
//						currentState.setString("EDIT MODE");
//					}
//				}
//				else if (!continousPainting && !boxSelection && !collisionsMode) {
//					switch (event.mouseButton.button) {
//					case sf::Mouse::Left: {
//						auto mousePos = sf::Mouse::getPosition(window);
//						auto translatedPos = window.mapPixelToCoords(mousePos);
//						for (int i = 0; i < gridSize; i++) {
//							for (int j = 0; j < gridSize; j++) {
//								if (grid[i][j].checkMouseClick(translatedPos)) {
//									if (placePlayer) { 
//										currentState.setString("PLACE MODE"); 
//										archer = new Player(grid[i][j].shape.getPosition());
//									} 
//									else grid[i][j].changeTexture(&tileset, currentTile);
//								}
//							}
//						}
//						break;
//					}
//					case sf::Mouse::Right: {
//						auto mousePos = sf::Mouse::getPosition(window);
//						auto translatedPos = window.mapPixelToCoords(mousePos);
//						std::cout << "Position: " << translatedPos.x << " , " << translatedPos.y << std::endl;
//						for (int i = 0; i < gridSize; i++) {
//							for (int j = 0; j < gridSize; j++) {
//								if (grid[i][j].checkMouseClick(translatedPos)) {
//									grid[i][j].changeColor(sf::Color::White);
//									grid[i][j].changeTexture(nullptr);
//								}
//							}
//						}
//						break;
//					}
//					}
//				}
//				else if (boxSelection) {
//					currentState.setString("BOX SELECTION");
//					mouseDrag = true;
//					box.setPosition(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
//				}
//			}
//
//			if (event.type == sf::Event::MouseButtonReleased && !playMode) {
//				if (continousPainting) mouseLocked = false;
//				if (collisionsMode) {
//		
//				}
//				if (boxSelection) {
//					mouseDrag = false;
//					box.setSize(window.mapPixelToCoords({ event.mouseButton.x, event.mouseButton.y }) - box.getPosition());
//					for (int i = 0; i < gridSize; i++) {
//						for (int j = 0; j < gridSize; j++) {
//							if (grid[i][j].shape.getGlobalBounds().intersects(box.getGlobalBounds())) {
//								grid[i][j].changeTexture(&tileset, currentTile);
//							}
//						}
//					}
//					boxSelection = false;
//				}
//
//				box.setSize({ 0,0 });
//				box.setPosition({ 0,0 });
//			}
//
//
//			if (event.type == sf::Event::MouseMoved && !playMode) {
//				if (boxSelection) {
//					if (mouseDrag) {
//						std::cout << "box pos: " << event.mouseMove.x << " , " << event.mouseMove.y << std::endl;
//						box.setSize(window.mapPixelToCoords({ event.mouseMove.x, event.mouseMove.y }) - box.getPosition());
//					}
//				}
//			}
//
//			if (continousPainting) {
//				currentState.setString("PAITING MODE");
//				if (mouseLocked) {
//					if (lastMousePos != sf::Mouse::getPosition()) {
//						auto mousePos = sf::Mouse::getPosition(window);
//						auto translatedPos = window.mapPixelToCoords(mousePos);
//						std::cout << "Position: " << translatedPos.x << " , " << translatedPos.y << std::endl;
//						for (int i = 0; i < gridSize; i++) {
//							for (int j = 0; j < gridSize; j++) {
//								if (grid[i][j].checkMouseClick(translatedPos)) {
//									grid[i][j].changeTexture(&tileset, currentTile);
//								}
//							}
//						}
//						lastMousePos = sf::Mouse::getPosition();
//					}
//				}
//			}
//			
//			if (event.type == sf::Event::MouseWheelScrolled && !playMode) {
//				if(event.mouseWheelScroll.delta > 0)
//					camera.zoom(1.f / 1.1f);
//				else if (event.mouseWheelScroll.delta < 0)
//					camera.zoom(1.1f);
//			}
//		}
//
//		ImGui::SFML::Update(window, clock.restart());
//
//		ImGui::Begin("Info");
		//if(ImGui::BeginMenu("File...")) {
		//	if (ImGui::Button("New...")) {
		//		currentState.setString("CLEAR SCENE");
		//		int howManyCleaned = 0;
		//		for (int i = 0; i < gridSize; i++) {
		//			for (int j = 0; j < gridSize; j++) {
		//				if (grid[i][j].hasTexture) {
		//					backgroundColor = sf::Color::White;
		//					grid[i][j].changeTexture(nullptr);
		//					grid[i][j].changeColor(sf::Color::White);
		//					howManyCleaned++;
		//				}
		//			}
		//		}
		//		currentState.setString("CLEANED " + howManyCleaned);
		//		std::cout << "Cleaned " << howManyCleaned << " tiles!\n";
		//	}
		//	ImGui::Button("Save...");
		//	ImGui::EndMenu();
		//}
//		ImGui::End();
//
//		if (window.hasFocus() && !playMode) {
//			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
//				camera.move(0, -10);
//			}
//
//			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
//				camera.move(0, 10);
//			}
//
//			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
//				camera.move(-10, 0);
//			}
//
//			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
//				camera.move(10, 0);
//			}
//
//			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z)) {
//				previewTile.rotate(10);
//			}
//
//			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::X)) {
//				previewTile.rotate(-10);
//			}
//
//			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num1)) {
//				currentTile = 0;
//				previewTile.setTextureRect(sf::IntRect(0, 0, 16, 16));
//			}
//			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num2)) {
//				currentTile = 1;
//				previewTile.setTextureRect(sf::IntRect(16, 0, 16, 16));
//			}
//			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num3)) {
//				currentTile = 2;
//				previewTile.setTextureRect(sf::IntRect(32, 0, 16, 16));
//			}
//			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num4)) {
//				currentTile = 3;
//				previewTile.setTextureRect(sf::IntRect(48, 0, 16, 16));
//			}
//			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num5)) {
//				currentTile = 4;
//				previewTile.setTextureRect(sf::IntRect(64, 0, 16, 16));
//			}
//			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num6)) {
//				currentTile = 5;
//				previewTile.setTextureRect(sf::IntRect(80, 0, 16, 16));
//			}
//		}
//
//		if (playMode) {
//			currentState.setString("PLAY MODE");
//			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
//				archer->body.move(0, -10);
//				for (int i = 0; i < colliders.size(); i++) {
//					if (archer->body.getGlobalBounds().intersects(colliders[i].getBounds())) {
//						archer->body.move(0, 10);
//					}
//				}
//			}
////
//			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
//				archer->body.move(0, 10);
//				for (int i = 0; i < colliders.size(); i++) {
//					if (archer->body.getGlobalBounds().intersects(colliders[i].getBounds())) {
//						archer->body.move(0, -10);
//					}
//				}
//			}
//
//			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
//				archer->body.move(-10, 0);
//				for (int i = 0; i < colliders.size(); i++) {
//					if (archer->body.getGlobalBounds().intersects(colliders[i].getBounds())) {
//						archer->body.move(10, 0);
//					}
//				}
//			}
//
//			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
//				archer->body.move(10, 0);
//				for (int i = 0; i < colliders.size(); i++) {
//					if (archer->body.getGlobalBounds().intersects(colliders[i].getBounds())) {
//						archer->body.move(-10, 0);
//					}
//				}
//			}
//
//		}
//		
//		auto previewTilePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
//		previewTile.setPosition(((int)(previewTilePos.x / previewTile.getSize().x))*64, ((int)(previewTilePos.y / previewTile.getSize().y)) * 64);
//		previewTile.setFillColor(sf::Color(255, 255, 255, 100));
//
//		window.clear(backgroundColor);
//
//		for (int i = 0; i < gridSize; i++) {
//			for (int j = 0; j < gridSize; j++) {
//				window.draw(grid[i][j].shape);
//			}
//		}
//
//		if (boxSelection) {
//			window.draw(box);
//		}
//		else {
//			box.setPosition({ 0,0 });
//			box.setSize({ 0,0 });
//			if (!playMode) {
//				window.draw(previewTile);
//				for (auto & element : colliders) {
//					window.draw(element);
//				}
//				if(archer != nullptr)
//				window.draw(archer->body);
//			}
//		}
//
		//if (archer != nullptr)
		//	window.draw(archer->body);
//
//
//		window.draw(currentState);
//
//		ImGui::SFML::Render(window);
//		window.display();
//	}
//
//	ImGui::SFML::Shutdown();
//}

int main() {
	Game game;
	game.pushState<EditorState>(game, "Brick Engine - Level editor");
	game.run();
}
