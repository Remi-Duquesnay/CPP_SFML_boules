#pragma once
#include "point.h"
#include "MapArea.h"
#include <SFML/Graphics.hpp>

class MapArea;
class Ball :
    public Point
{
private:
    int _ballId;
    float _radius;
    sf::CircleShape _projection;
    int _xPos;
    int _yPos;
    sf::Color _color;
    sf::Vector2f _velocity;
    float _ax;
    float _ay;
public:
    Ball();
    Ball(int id, float r, sf::CircleShape Projection, int xp, int yp, sf::Color color);

    void Move(sf::Vector2f, sf::Vector2f, MapArea&);
    sf::Vector2f CollisionDetection(sf::Vector2f, sf::Vector2f, sf::Vector2f, MapArea&);
    void SetVelocity(sf::Vector2f);
    sf::CircleShape GetProjection();
    sf::Vector2f GetPos();
    sf::Vector2f GetVelocity();
    int GetX();
    int GetY();
    int GetId();
};

