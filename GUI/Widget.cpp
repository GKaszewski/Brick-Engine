#include "Widget.hpp"

Widget::Text::Text() {
    ResourceHolder::get().fonts.add("roboto");
    setCharacterSize(25);
    setOutlineColor(sf::Color::Black);
    setFillColor(sf::Color::White);
    setFont(ResourceHolder::get().fonts.get("roboto"));
}

bool Widget::Rectangle::isMouseOver(const sf::RenderWindow& window) const {
    auto pos = sf::Mouse::getPosition(window);
    return getGlobalBounds().contains((float)pos.x, (float)pos.y);
}

bool Widget::Rectangle::isClicked(sf::Event e, const sf::RenderWindow& window) {
    if (isMouseOver(window)) {
        if (e.type == sf::Event::MouseButtonPressed) {
            return e.mouseButton.button == sf::Mouse::Left;
        }
    }
    return false;
}
