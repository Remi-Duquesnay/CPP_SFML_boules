#include "MapArea.h"
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;
MapArea::MapArea()
{
}

MapArea::MapArea(sf::Vector2f size, sf::Vector2f position)
{
	this->_size = size;
	this->_position = position;

	// Create a vector of "lines" of the ball area (it size is = to the number of vertical coord of the area), 
	// containing vectors of "collumns" of the ball (it size is = to the number of vertical coord of the area)
	// the "0" at the end is to préinitialize the data of the "collumn" vectors to 0
	this->_matrix = vector<vector<Ball*>>(size.x, vector<Ball*>(size.y, nullptr));
}


void MapArea::AddBall(Ball* ball, sf::Vector2f ballMinPos, sf::Vector2f ballMaxPos) {
	//substract the offset of the area position to set the right position in the matrix
	int x = ball->GetProjection().getPosition().x - this->_position.x;
	int y = ball->GetProjection().getPosition().y - this->_position.y;

	this->_matrix[x][y] = ball;
}

Ball* MapArea::GetBallAroundPos(sf::Vector2f pos, int radius) {
	pos.x -= this->_position.x;
	pos.y -= this->_position.y;
	int minX = pos.x - ((radius * 2) - 1);
	int maxX = pos.x + ((radius * 2) - 1);
	int minY = pos.y - ((radius * 2) - 1);
	int maxY = pos.y + ((radius * 2) - 1);

	if (minX < 0) {
		minX = 0;
	}
	if (maxX > this->_size.x) {
		maxX = this->_size.x;
	}
	if (minY < 0) {
		minY = 0;
	}
	if (maxY > this->_size.y) {
		maxY = this->_size.y;
	}

	for (int i = minX; i < maxX; i++) {
		for (int j = minY; j < maxY; j++) {
			Ball* test = this->_matrix[i][j];
			if (test != nullptr) {
				return this->_matrix[i][j];
			}
		}
	}
	return nullptr;
}

void MapArea::MoveBall(sf::Vector2f oldPos, sf::Vector2f newPos) {
	oldPos.x -= this->_position.x;
	oldPos.y -= this->_position.y;
	newPos.x -= this->_position.x;
	newPos.y -= this->_position.y;
	Ball* test1 = this->_matrix[newPos.x][newPos.y];
	Ball* test2 = this->_matrix[oldPos.x][oldPos.y];
	this->_matrix[newPos.x][newPos.y] = this->_matrix[oldPos.x][oldPos.y];
	this->_matrix[oldPos.x][oldPos.y] = nullptr;
	Ball* test11= this->_matrix[newPos.x][newPos.y];
	Ball* test22 = this->_matrix[oldPos.x][oldPos.y];
}
