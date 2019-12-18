#include "ResourceHolder.hpp"

ResourceHolder & ResourceHolder::get() {
    static ResourceHolder holder;
    return holder;
}

inline ResourceHolder::ResourceHolder()
    : fonts("fonts", "ttf"), textures("textures", "png") {}