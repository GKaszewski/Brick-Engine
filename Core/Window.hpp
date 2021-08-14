#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "State.hpp"
#include "../Managers/EventManager.hpp"

class Window {
public:
	Window();
	Window(const std::string& title, const sf::Vector2u& size);
	~Window();

	void beginDraw();
	void endDraw();

	void update();

	bool getIsDone();
	bool getIsFullscreen();
	sf::Vector2u getWindowSize();
	inline sf::RenderWindow& getWindow() { return window; }
	inline EventManager* getEventManager() { return &eventManager; }

	void toggleFullscreen(EventDetails* details);
	void draw(sf::Drawable& drawable);
	void close(EventDetails* details = nullptr);
private:
	void setup(const std::string& title, const sf::Vector2u& size);
	void destroy();
	void create();

	sf::RenderWindow window;
	sf::Vector2u windowSize;
	std::string windowTitle;
	EventManager eventManager;

	bool isDone;
	bool isFullscreen;
	bool isFocused;
};

