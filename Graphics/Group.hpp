#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <functional>

class Group : public sf::Drawable {
public:
    Group();
    virtual ~Group() = default;

    virtual void draw(sf::RenderTarget & target, sf::RenderStates states) const;

    const sf::Drawable& operator[](std::size_t index);
    std::size_t push_back(const sf::Drawable & drawable);
    const sf::Drawable& pop_back();
private:
    std::vector<std::reference_wrapper<const sf::Drawable>> m_drawables;
};

