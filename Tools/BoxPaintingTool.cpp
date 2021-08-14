#include "BoxPaintingTool.hpp"
#include <iostream>
#include <cmath>

BoxPaintingTool::BoxPaintingTool(Game& game, int blockID, Grid& grid): game(game), grid(grid) {
	this->blockID = blockID;
	previewBox.setFillColor(sf::Color::Green);
}

void BoxPaintingTool::startDragging() {
	mouseDragging = true;
	auto pos = game.getWindow()->getWindow().mapPixelToCoords(sf::Mouse::getPosition(game.getWindow()->getWindow()));
	auto tileSize = grid.CellSize();
	pos.x = roundf(pos.x / tileSize.x) * tileSize.x;
	pos.y = roundf(pos.y / tileSize.y) * tileSize.y;
	previewBox.setPosition(pos);
}

void BoxPaintingTool::onDrag(sf::Event e) {
	if (mouseDragging) {
		auto size = game.getWindow()->getWindow().mapPixelToCoords({ e.mouseMove.x, e.mouseMove.y }) - previewBox.getPosition();
		auto tileSize = grid.CellSize();
		size.x = roundf(size.x / tileSize.x) * tileSize.x;
		size.y = roundf(size.y / tileSize.y) * tileSize.y;
		previewBox.setSize(size);
	}
}

SelectionData BoxPaintingTool::endDragging(sf::Event e) {
	mouseDragging = false;
	auto tileSize = grid.CellSize();
	SelectionData data;

	auto topLeftX = previewBox.getPosition().x;
	auto topLeftY = previewBox.getPosition().y;
	auto bottomRightX = topLeftX + previewBox.getSize().x;
	auto bottomRightY = topLeftY + previewBox.getSize().y;
	for (auto x = topLeftX; x < bottomRightX; x += tileSize.x) {
		for (auto y = topLeftY; y < bottomRightY; y += tileSize.y) {
			data.data.insert({ std::to_string(x) + "-" + std::to_string(y), blockID});
		}
	}

	previewBox.setSize({ 0.0f, 0.0f });
	previewBox.setPosition({ 0.0f, 0.0f });

	return data;
}

void BoxPaintingTool::drawPreview(sf::RenderTarget& target) {
	target.draw(previewBox);
}