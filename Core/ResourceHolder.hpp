#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "ResourceManager.hpp"

class ResourceHolder : public sf::NonCopyable{
public:
    static ResourceHolder & get();

    ResourceManager<sf::Font> fonts;
    ResourceManager<sf::Texture> textures;
private:
    ResourceHolder();
};