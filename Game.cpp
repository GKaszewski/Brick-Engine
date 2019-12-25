#include "Game.hpp"
#include "Managers/PhysicsManager.hpp"

Game::Game() : window({1280, 720}, "Brick Engine"){   
    window.setPosition({window.getPosition().x, 0});
    window.setFramerateLimit(60);

    //loadIcon();
}

Game::Game(unsigned width, unsigned height, bool vsync) : window({width, height}, "Brick Engine"){
    window.setVerticalSyncEnabled(vsync);

    //loadIcon();
}

Game::Game(unsigned width, unsigned height, const std::string & title ,bool vsync) : window({width, height}, title) {
    Game(width, height, vsync);
}

void Game::loadIcon(){
    icon.loadFromFile("assets/icon.png");
    window.setIcon(32, 32, icon.getPixelsPtr());
}

void Game::run(){
    constexpr unsigned TPS = 30;
    const sf::Time timePerUpdate = sf::seconds(1.0f / float(TPS));
    unsigned ticks = 0;

    sf::Clock timer;
    auto lastTime = sf::Time::Zero;
    auto lag = sf::Time::Zero;

    while(window.isOpen() && !states.empty()){
        auto & state = getCurrentState();

        auto time = timer.getElapsedTime();
        auto elapsed = time - lastTime;
        lastTime = time;
        lag += elapsed;
        
        PhysicsManager::getInstance()->world->Step(1 / 60.0f, 8, 3);

        state.handleInput();
        state.update(elapsed);

        while (lag >= timePerUpdate){
            ticks++;
            lag -= timePerUpdate;
            state.fixedUpdate(elapsed);
        }
        
        window.clear();
        state.render(window);
        window.display();

        handleEvents();
        tryPop();
    }
}

void Game::tryPop() {
    if (shouldPop) {
        shouldPop = false;
        if (shouldExit) {
            states.clear();
            return;
        } else if (shouldChangeState) {
            shouldChangeState = false;
            states.pop_back();
            pushState(std::move(change));
            return;
        }

        states.pop_back();
        if (!states.empty()) {
            getCurrentState().onOpen();
        }
    }
}

void Game::handleEvents() {
    sf::Event e;

    while (window.pollEvent(e)) {
        getCurrentState().handleEvent(e);
        switch (e.type) {
        case sf::Event::Closed:
            window.close();
            break;

        default:
            break;
        }
    }
}

void Game::pushState(std::unique_ptr<State> state) {
    states.push_back(std::move(state));
    getCurrentState().onOpen();
}

void Game::exitGame() {
    shouldPop = true;
    shouldExit = true;
}

void Game::resizeWindow(unsigned width, unsigned height) {
    window.close();
    window.create({width, height}, "Brick Engine");
}