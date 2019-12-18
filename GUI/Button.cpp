#include "Button.hpp"
#include <iostream>

Button::Button() {
	button.setOutlineThickness(2);
	button.setOutlineColor(sf::Color::Black);
	button.setFillColor(sf::Color::White);
	button.setSize({256, 64});
    button.setPosition({ 100, 100 });

    text.setString("button");
    text.setFillColor(sf::Color::White);
    text.setOutlineColor(sf::Color::Black);
    text.setOutlineThickness(3);
    text.setFont(ResourceHolder::get().fonts.get("roboto"));

    std::cout << "Button is created!\n";
}

Button::~Button() {
    std::cout << "Button is destroyed!\n";
}

void Button::setFunction(std::function<void(void)> function) {
	this->function = function;
}

void Button::setText(const std::string& text) {
	this->text.setString(text);
	updateText();
}

void Button::setTexture(const sf::Texture& texture) {
	button.setTexture(&texture);
}

void Button::handleEvents(sf::Event e, const sf::RenderWindow& window) {
    auto pos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    switch (e.type) {
    case sf::Event::MouseButtonPressed:
        switch (e.mouseButton.button) {
        case sf::Mouse::Left:
            if (button.getGlobalBounds().contains((float)pos.x, (float)pos.y)) {
                function();
            }

        default:
            break;
        }

    default:
        break;
    }
}

void Button::render(sf::RenderTarget& renderer) {
    renderer.draw(button);
    renderer.draw(text);
}

void Button::setPosition(const sf::Vector2f& pos) {
    position = pos;

    button.setPosition(position);
    text.setPosition(position);

    updateText();
}

sf::Vector2f Button::getSize() const { return button.getSize(); }

void Button::updateText() {
    text.setOrigin(text.getGlobalBounds().width / 2, text.getGlobalBounds().height / 2);
    text.move(button.getGlobalBounds().width / 2.0f, button.getGlobalBounds().height / 2.5f);
}
