#include "Window.hpp"

Window::Window() {
    setup("Brick Engine", { 1280, 704 });
}

Window::Window(const std::string& title, const sf::Vector2u& size) {
    setup(title, size);
}

Window::~Window() {
    destroy();
}

void Window::beginDraw() {
    window.clear(sf::Color::Black);
}

void Window::endDraw() {
    window.display();
}

void Window::update() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::LostFocus) {
            isFocused = false;
            eventManager.setFocus(false);
        }
        else if (event.type == sf::Event::GainedFocus) {
            isFocused = true;
            eventManager.setFocus(true);
        }
        eventManager.handleEvent(event);
    }
    eventManager.update();
}

bool Window::getIsDone() {
    return isDone;
}

bool Window::getIsFullscreen() {
    return isFullscreen;
}

sf::Vector2u Window::getWindowSize() {
    return windowSize;
}

void Window::toggleFullscreen(EventDetails* details) {
    isFullscreen = !isFullscreen;
    destroy();
    create();

}

void Window::draw(sf::Drawable& drawable) {
    window.draw(drawable);
}

void Window::close(EventDetails* details) {
    isDone = true;
}

void Window::setup(const std::string& title, const sf::Vector2u& size) {
    windowTitle = title;
    windowSize = size;
    isFullscreen = false;
    isDone = false;
    isFocused = true;
    eventManager.addCallback("Fullscreen_toggle", &Window::toggleFullscreen, this);
    eventManager.addCallback("Window_close", &Window::close, this);
    create();

}

void Window::destroy() {
    window.close();
}

void Window::create() {
    auto style = (isFullscreen ? sf::Style::Fullscreen
        : sf::Style::Default);
    window.create({ windowSize.x, windowSize.y, 32 },
        windowTitle, style);
    window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(true);
}
