#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

#include "State.hpp"

class Game : public sf::NonCopyable{
public:
    Game();
    Game(unsigned width, unsigned height, bool vsync);
    Game(unsigned width, unsigned height, const std::string & title ,bool vsync);
    ~Game() = default;

    void run();

    template <typename T> 
    void initGame();

    void pushState(std::unique_ptr<State> state);
    inline void popState() { shouldPop = true; }
    void exitGame();

    template<typename T, typename... Args>
    void pushState(Args && ... args);
    
    template<typename T, typename... Args>
    void changeState(Args && ... args);

    inline sf::RenderWindow & getWindow() { return window; }
    void resizeWindow(unsigned width, unsigned height);

private:
    sf::RenderWindow window;

    void handleEvents();
    void tryPop();
    void loadIcon();
    inline State & getCurrentState() { return *states.back(); }

    std::vector<std::unique_ptr<State>> states;

    bool shouldPop = false;
    bool shouldExit = false;
    bool shouldChangeState = false;
    std::unique_ptr<State> change;
    sf::Image icon;

};

template <typename T> 
inline void Game::initGame(){
    this->pushState<T>(*this);
}

template<typename T, typename... Args>
void Game::pushState(Args && ... args){
    pushState(std::make_unique<T>(std::forward<Args>(args)...));
}

template<typename T, typename... Args>
void Game::changeState(Args && ... args){
    change = std::make_unique<T>(std::forward<Args>(args)...);
    shouldPop = true;
    shouldChangeState = true;
}