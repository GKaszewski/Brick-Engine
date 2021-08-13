#pragma once
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <vector>
#include <string>

namespace utils {
	const int GROUND_ID = 0;

	b2Vec2 convert_sfml_vec_to_b2vec(sf::Vector2f vec);
	sf::Vector2f convert_b2vec_to_sfml_vec(b2Vec2 vec);

	std::vector<std::string> split(const std::string& s, char delim);
}