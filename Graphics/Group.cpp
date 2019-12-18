#include "Group.hpp"

Group::Group() : m_drawables{} {

}

void Group::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    for (const auto& drawable : m_drawables) {
        target.draw(drawable, states);
    }
}

const sf::Drawable& Group::operator[](std::size_t index) {
    return m_drawables[index];
}

std::size_t Group::push_back(const sf::Drawable& drawable) {
    m_drawables.push_back(drawable);
    return m_drawables.size() - 1;
}

const sf::Drawable& Group::pop_back() {
    const auto& drawable = m_drawables.back();
    m_drawables.pop_back();
    return drawable;
}
