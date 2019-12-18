#pragma once
#include "Widget.hpp"
#include <functional>

class Button : public Widget {
public:

    Button();
    ~Button();

    void setFunction(std::function<void(void)> function);

    void setText(const std::string& text);

    void setTexture(const sf::Texture& texture);

    void handleEvents(sf::Event e, const sf::RenderWindow& window) override;

    void render(sf::RenderTarget& renderer) override;
    
    void setPosition(const sf::Vector2f& pos) override;

    sf::Vector2f getSize() const override;

private:
    void updateText();

    sf::Vector2f position;
    Rectangle button;
    Text text;
    std::function<void(void)> function = []() {};
};

inline std::unique_ptr<Button> makeButton()
{
    return std::make_unique<Button>();
}


