#include "Ball.h"
#include <stdlib.h> 

#include <iostream>
using namespace std;

Ball::Ball() {

}

Ball::Ball(int id, float radius, sf::CircleShape projection, int xPos, int yPos, sf::Color color)
{
	this->_ballId = id;
	this->_radius = radius;
	this->_xPos = xPos;
	this->_yPos = yPos;
	this->_color = color;
	do {
		this->_velocity.x = rand() % 3 + -1;
	} while (_velocity.x == 0);
	do {
		this->_velocity.y = rand() % 3 + -1;
	} while (_velocity.y == 0);
	this->_ax = 0;
	this->_ay = 0;

	projection.setRadius(radius);
	projection.setPosition(sf::Vector2f(_xPos, _yPos));
	projection.setFillColor(color);
	projection.setOrigin(_radius, _radius);

	this->_projection = projection;
}

void Ball::Move(sf::Vector2f ballMinPos, sf::Vector2f ballMaxPos, MapArea& mapArea) {
	sf::Vector2f pos = this->GetPos();
	sf::Vector2f velocity = this->GetVelocity();

	sf::Vector2f newPos = sf::Vector2f(pos.x + velocity.x, pos.y + velocity.y);
	sf::Vector2f newVelocity = CollisionDetection(newPos, ballMinPos, ballMaxPos, mapArea);

	if (newVelocity != velocity) {
		sf::Vector2f newPos = sf::Vector2f(pos.x + newVelocity.x, pos.y + newVelocity.y);
	}
	this->_xPos = newPos.x;
	this->_yPos = newPos.y;
	this->_projection.setPosition(newPos);
	mapArea.MoveBall(pos, newPos);
}

sf::Vector2f Ball::CollisionDetection(sf::Vector2f newPos, sf::Vector2f ballMinPos, sf::Vector2f ballMaxPos, MapArea& mapArea) {
	sf::Vector2f velocity = this->GetVelocity();
	sf::Vector2f newVelocity = this->GetVelocity();
	Ball* otherBall = mapArea.GetBallAroundPos(newPos, this->_radius);

	if (newPos.x - _radius <= ballMinPos.x || newPos.x + _radius >= ballMaxPos.x) {
		newVelocity.x = velocity.x * -1;
	}
	else if (newPos.y - _radius <= ballMinPos.y || newPos.y + _radius >= ballMaxPos.y) {
		newVelocity.y = velocity.y * -1;
	}
	else if (otherBall != nullptr) {
		otherBall->SetVelocity(sf::Vector2f(-otherBall->GetVelocity().x, -otherBall->GetVelocity().y));
		newVelocity = sf::Vector2f(-this->GetVelocity().x, -this->GetVelocity().y);
	}
	this->SetVelocity(newVelocity);
	return newVelocity;
}

sf::Vector2f Ball::GetVelocity()
{
	return this->_velocity;
}
int Ball::GetX()
{
	return this->_xPos;
}
int Ball::GetY()
{
	return this->_yPos;
}
int Ball::GetId()
{
	return this->_ballId;
}
void Ball::SetVelocity(sf::Vector2f velocity)
{
	this->_velocity = velocity;
}
sf::CircleShape Ball::GetProjection()
{
	return this->_projection;
}
sf::Vector2f Ball::GetPos() {
	return sf::Vector2f(this->_xPos, this->_yPos);
}