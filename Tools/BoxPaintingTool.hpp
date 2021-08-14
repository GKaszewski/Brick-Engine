#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include "../Core/Game.hpp"
#include "../Graphics/Grid.hpp"

struct SelectionData {
	std::unordered_map<std::string, int> data;
};

class BoxPaintingTool {
public:
	BoxPaintingTool(Game& game, int blockID, Grid& grid);

	void startDragging();
	void onDrag(sf::Event e);
	SelectionData endDragging(sf::Event e);

	void drawPreview(sf::RenderTarget& target);

	int& BlockID() { return blockID; }

private:
	Game& game;
	Grid& grid;
	bool mouseDragging = false;
	sf::RectangleShape previewBox;
	int blockID;
};