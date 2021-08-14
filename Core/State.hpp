#pragma once
#include <SFML/Graphics.hpp>
//#include "Game.hpp"

class Game;

class State : public sf::NonCopyable{
public:
    State(Game & game, const char *name) : game(&game), name(name) {}

    virtual ~State() = default;

    virtual void onOpen() {}
    virtual void handleEvent(sf::Event e) {}
    virtual void handleInput() {}
    virtual void update(sf::Time deltaTime) {}
    virtual void fixedUpdate(sf::Time deltaTime) {}
    virtual void render(sf::RenderTarget &renderer) = 0;

    const std::string &getName() { return name; }

protected:
    Game * game;
    std::string name;
};