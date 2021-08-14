#include "Game.hpp"
#include "../Managers/PhysicsManager.hpp"

Game::Game() : window() {
    //loadIcon();
}

Game::Game(unsigned width, unsigned height) : window("Brick Engine", {width, height}) {
    //loadIcon();
}

void Game::loadIcon(){
    icon.loadFromFile("assets/icon.png");
    window.getWindow().setIcon(32, 32, icon.getPixelsPtr());
}

void Game::run(){
    constexpr unsigned TPS = 30;
    const sf::Time timePerUpdate = sf::seconds(1.0f / float(TPS));
    unsigned ticks = 0;

    sf::Clock timer;
    auto lastTime = sf::Time::Zero;
    auto lag = sf::Time::Zero;

    while(!window.getIsDone() && !states.empty()){
        auto & state = getCurrentState();

        auto time = timer.getElapsedTime();
        auto elapsed = time - lastTime;
        lastTime = time;
        lag += elapsed;

        state.handleInput();
        PhysicsManager::getInstance()->world->Step(1 / 60.0f, 15, 15);
        state.update(elapsed);

        while (lag >= timePerUpdate){
            ticks++;
            lag -= timePerUpdate;
            state.fixedUpdate(elapsed);
        }
        
        window.beginDraw();
        state.render(window.getWindow());
        window.endDraw();

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
    window.update();
    sf::Event e;
    while (window.getWindow().pollEvent(e)) {
        getCurrentState().handleEvent(e);
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