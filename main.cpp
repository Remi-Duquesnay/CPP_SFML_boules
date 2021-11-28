#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <algorithm>

#include "point.h"
#include "Ball.h"
#include "MapArea.h"

using namespace std;

bool checkFileExist(string);
vector<Point> initPoints(string, int);
vector<Ball*> createBalls(vector<Point>, sf::Vector2f, sf::Vector2f, MapArea&);
sf::RectangleShape createBallArea(sf::Vector2f, sf::Vector2f, float);

int main() {

	string windowTitle = "Bouncy balls"; // title of the window

	string fontFile = "MeltedMonster.ttf"; // name of the file of the font used

	string initFile = "initial.txt"; // name of the file of point/ball initialization


	// size of the window
	int windowWidth = 1000;
	int windowHeight = 1000;

	int pointMaxPosition = 8000;

	vector<Point> points;
	vector<Ball*> balls;



	sf::RectangleShape ballArea;
	sf::Vector2f ballAreaSize = sf::Vector2f(800.f, 800.f);
	sf::Vector2f ballAreaPos = sf::Vector2f(10.f, 100.f);
	sf::Vector2f ballMinPos = sf::Vector2f(ballAreaPos.x, ballAreaPos.y); // calculate the min position for the ball to be inside the area
	sf::Vector2f ballMaxPos = sf::Vector2f(ballAreaPos.x + ballAreaSize.x, ballAreaPos.y + ballAreaSize.y); // same but for the max position
	float ballAreaOutline = 5.f;

	if (!checkFileExist(initFile)) {
		return 0;
	}

	ballArea = createBallArea(ballAreaSize, ballAreaPos, ballAreaOutline);
	MapArea mapArea = MapArea(ballAreaSize, ballAreaPos);

	points = initPoints(initFile, pointMaxPosition);
	balls = createBalls(points, ballMinPos, ballMaxPos, mapArea);

	sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), windowTitle);

	sf::Font font;
	font.loadFromFile(fontFile);
	sf::Text textClock;
	textClock.setFont(font);
	textClock.setPosition(sf::Vector2f(10.f, 10.f));
	textClock.setCharacterSize(24);
	textClock.setFillColor(sf::Color::White);

	sf::Text testKeys;
	testKeys.setFont(font);
	testKeys.setPosition(sf::Vector2f(10.f, 30.f));
	testKeys.setCharacterSize(24);
	testKeys.setFillColor(sf::Color::White);

	sf::Clock clock;
	float elapsedTime = 0;
	float timeSave = 0;
	bool pause = false;
	string buffer = "";

	while (window.isOpen())
	{


		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::LostFocus)
			{
				pause = true;
				timeSave += clock.getElapsedTime().asSeconds();
			}
			if (event.type == sf::Event::GainedFocus)
			{
				pause = false;
				clock.restart();
			}
			if (event.type == sf::Event::TextEntered) {
				char keypressed = event.text.unicode;
				buffer += keypressed;
				testKeys.setString("Keys pressed : " + buffer);
			}
		}
		window.clear();
		window.draw(ballArea);


		for (auto itr = balls.begin(); itr != balls.end(); itr++) {

			window.draw((*itr)->GetProjection());

		}

		window.draw(textClock);

		window.draw(testKeys);

		if (pause == false) {
			if ((clock.getElapsedTime().asMilliseconds() % 100) <= 0) {
				for (auto itr = balls.begin(); itr != balls.end(); itr++) {


					(*itr)->Move(ballMinPos, ballMaxPos, mapArea);
				}
			}
			elapsedTime = timeSave + clock.getElapsedTime().asSeconds();
			textClock.setString("Elapsed time : " + to_string(elapsedTime));
		}
		

		window.display();
	}

	return 0;
}

bool checkFileExist(string fileName) {
	try {
		fstream file; // declaration of the variable that will contain the file that we want to interact with.
		file.open(fileName, ios::in); // open the file in read mode (ios::in)
		if (file.is_open()) { // check if the file is opened (mean that the file exist)
			file.close(); // close the file
			return  true;
		}
		else {
			throw "File not found";
		}
	}
	catch (const char* exception) {
		cout << exception << endl;
		return false;
	}
}

vector<Point> initPoints(string fileName, int pointMaxPosition) {
	vector<Point> points; // declaration of the vector that will contain all the points
	fstream file; // declaration of the variable that will contain the file that we want to interact with.
	file.open(fileName, ios::in); // open the file in read mode (ios::in)
	if (file.is_open()) { // check if the file is open

		string line; // declaration of the variable that will contain the line that we obtain.

		// counting the numbers of lines of the file to reserve the final CAPACITY of the vector in memory
		int nbLine = 0;
		while (getline(file, line)) {
			nbLine++;
		}
		file.clear(); // clear the flags of the stream (in this case remove the End-of-File flag)
		file.seekg(0, std::ios::beg); // reposition the "iterator" of the stream at the first character (in this case the start of the file)

		points.reserve(nbLine); // change/reserve the capacity of the vector to match the number of lignes of the file

		//Loop to read each line of the file and initialize a point with the data obtained from each line
		while (getline(file, line)) {
			stringstream linestream(line); // convert the string into a stream 
			string sId, sX, sY; // declaring the variables that will get the data as strings
			// getline extract everything until a ';' (or the end of the stream) and store it into a variable
			// the data extracted and the delimiter (';') will be removed from the stream
			getline(linestream, sId, ';');
			getline(linestream, sX, ';');
			getline(linestream, sY); // no delimiter needed since we should be at the end of the stream

			// convert the strings obtained into integer
			int id = atoi(sId.c_str());
			int x = atoi(sX.c_str()); // + 10 to offset to stay in the rectangle
			int y = atoi(sY.c_str()); // + 100 to offset to stay in the rectangle

			// check if the point position on the x axis is not over the max or in the negative
			// if over we set it on the max position
			if (x > pointMaxPosition) {
				x = pointMaxPosition;
			}
			else if (x < 0) { // if in the negative we set it to 0
				x = 0;
			}

			// same but for y
			if (y > pointMaxPosition) {
				y = pointMaxPosition;
			}
			else if (y < 0) {
				y = 0;
			}

			Point point = Point(x, y, id); // create a new point object construct the object with his values

			points.push_back(point); // add the new object at the end of the vector

		}
		file.close(); // close the file
	}
	else {
		cout << "could not open file" << endl;
	}

	return points;
}

vector<Ball*> createBalls(vector<Point> points, sf::Vector2f ballMinPos, sf::Vector2f ballMaxPos, MapArea& mapArea) {

	vector<Ball*> balls;

	for (auto itr = points.begin(); itr != points.end(); itr++) {
		sf::CircleShape shape;
		float radius = 5.f;
		int xPos = ((*itr).Point::X() / 10) + ballMinPos.x;
		int yPos = ((*itr).Point::Y() / 10) + ballMinPos.y;
		int id = (*itr).Point::Id();

		if ((xPos + radius) >= ballMaxPos.x) {
			xPos = ballMaxPos.x - radius;
		}
		else if ((xPos - radius) <= ballMinPos.x) {
			xPos = ballMinPos.x + radius;
		}

		if ((yPos + radius) >= ballMaxPos.y) {
			yPos = ballMaxPos.y - radius;
		}
		else if ((yPos - radius) <= ballMinPos.y) {
			yPos = ballMinPos.y + radius;
		}

		sf::Color color = sf::Color(((*itr).Point::X() / 10) % 255, ((*itr).Point::Y() / 10) % 255, (((*itr).Point::X() / 10) + ((*itr).Point::Y() / 10)) % 255);
		Ball* ball = new Ball(id, radius, shape, xPos, yPos, color);
		balls.push_back(ball);
		mapArea.AddBall(ball, ballMinPos, ballMaxPos);
	}
	return balls;
}

sf::RectangleShape createBallArea(sf::Vector2f size, sf::Vector2f pos, float outligne) {

	sf::RectangleShape ballArea(size);
	ballArea.setPosition(pos);
	ballArea.setOutlineThickness(outligne);
	ballArea.setFillColor(sf::Color::Black);

	return ballArea;
}
