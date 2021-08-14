#pragma once
#include <SFML/Graphics.hpp>
#include "../Core/ResourceHolder.hpp"
#include <iostream>

class Player {
public:
	Player(sf::Vector2f pos = { 0,0 }) {
		ResourceHolder::get().textures.add("player");
		body.setPosition(pos);
		body.setSize({ 64, 128 });
		velocity = sf::Vector2f(0, -30);
		acceleration = sf::Vector2f(0, 9.81); //Earth's acceleration
		body.setTexture(&ResourceHolder::get().textures.get("player"));
		std::cout << "Player! " << body.getPosition().x << " , " << body.getPosition().y << std::endl;
	}

	enum class Direction { UP, DOWN, RIGHT, LEFT };

	sf::RectangleShape body;
	sf::Vector2f position, velocity, acceleration;
	void move(Direction direction, float speed) {
		switch (direction) {
		case Player::Direction::UP:
			body.move(0, -speed);
			break;
		case Player::Direction::DOWN:
			body.move(0, speed);
			break;
		case Player::Direction::RIGHT:
			body.move(speed, 0);
			break;
		case Player::Direction::LEFT:
			body.move(-speed, 0);
			break;
		default:
			break;
		}
	}
private:
};

	