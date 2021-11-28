#pragma once
#include <SFML/Graphics.hpp>

#include "Ball.h"
class Ball;
class MapArea
{
private:
	
	sf::Vector2f _size;
	sf::Vector2f _position;
	std::vector<std::vector<Ball*>> _matrix;

public:
	
	MapArea();
	MapArea(sf::Vector2f, sf::Vector2f);

	void AddBall(Ball*, sf::Vector2f, sf::Vector2f);

	Ball* GetBallAroundPos(sf::Vector2f, int radius);
	void MoveBall(sf::Vector2f, sf::Vector2f);
};

