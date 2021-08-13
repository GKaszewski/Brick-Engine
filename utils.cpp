#include "utils.hpp"
#include "Managers/PhysicsManager.hpp"
#include <sstream>

b2Vec2 utils::convert_sfml_vec_to_b2vec(sf::Vector2f vec) {
	auto b2vector = b2Vec2(vec.x / PhysicsManager::SCALE, vec.y / PhysicsManager::SCALE);
	return b2vector;
}

sf::Vector2f utils::convert_b2vec_to_sfml_vec(b2Vec2 vec) {
	auto sfml_vector = sf::Vector2f(vec.x * PhysicsManager::SCALE, vec.y * PhysicsManager::SCALE);
	return sfml_vector;
}

std::vector<std::string> utils::split(const std::string& s, char delim) {
    std::vector<std::string> result;
    std::stringstream ss(s);
    std::string item;

    while (std::getline(ss, item, delim)) {
        result.push_back(item);
    }

    return result;
}
