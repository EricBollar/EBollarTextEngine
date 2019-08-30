#include "EBollarTextEngine.h"
#include <iostream>

void esb::Sprite::setIndex(int i) {
	index = i;
}

int esb::Sprite::getIndex() {
	return index;
}

void esb::Sprite::setW(int width) {
	w = width;
}

void esb::Sprite::setH(int height) {
	h = height;
}

int esb::Sprite::getID() {
	return id;
}

std::string esb::Sprite::getName() {
	return name;
}

void esb::Sprite::setID(int i) {
	id = i;
}

void esb::Sprite::setName(std::string n) {
	name = n;
}

void esb::Sprite::Translate(int xChange, int yChange) {
	int xToGo = xChange; // this is for the x
	bool completedX = false;
	while (xToGo != 0 && !completedX) {
		bool restrictedRight = false;
		bool restrictedLeft = false;

		std::cout << map.size();
		for (int i = 0; i < map.size(); i++) { // checks if any of sprite's pixels are directly next to resolution border (out of scene's 2d array index) and therefore restricting movement in that dir.
			if (map.at(i).x - 1 < 0) {
				restrictedLeft = true;
			}
			if (map.at(i).x + 1 < 0) {
				restrictedRight = true;
			}
		}

		if (xToGo > 0) { // left-right
			if (!restrictedRight) {
				for (int i = 0; i < map.size(); i++) {
					map.at(i).x++;
				}
				xToGo--;
			}
			else {
				completedX = true;
			}
		} else if (xToGo < 0) {
			if (!restrictedLeft) {
				for (int i = 0; i < map.size(); i++) {
					map.at(i).x--;
				}
				xToGo++;
			}
			else {
				completedX = true;
			}
		}
	}

	
	int yToGo = yChange; // this is for the x
	bool completedY = false;
	while (yToGo != 0 && !completedY) {
		bool restrictedUp = false;
		bool restrictedDown = false;

		for (int i = 0; i < map.size(); i++) { // checks if any of sprite's pixels are directly next to resolution border (out of scene's 2d array index) and therefore restricting movement in that dir.
			if (map.at(i).y - 1 < 0) {
				restrictedUp = true;
			}
			if (map.at(i).y + 1 < 0) {
				restrictedDown = true;
			}
		}

		if (yToGo > 0) { // left-right
			if (!restrictedDown) {
				for (int i = 0; i < map.size(); i++) {
					map.at(i).y++;
				}
				yToGo--;
			}
			else {
				completedY = true;
			}
		}
		else if (yToGo < 0) {
			if (!restrictedUp) {
				for (int i = 0; i < map.size(); i++) {
					map.at(i).y--;
				}
				yToGo++;
			}
			else {
				completedY = true;
			}
		}
	}
}

void esb::Sprite::fillMap(esb::color c) {
	for (int i = 0; i < map.size(); i++) {
		map[i].c = c;
	}
}

void esb::Sprite::setMap(std::vector<esb::Pixel> m) {
	map = m;
}

std::vector<esb::Pixel> &esb::Sprite::getMap() {
	return map;
}

void esb::Sprite::setText(std::string t) {
	for (int i = 0; i < map.size(); i++) {
		if (i == t.size() || t == "") {
			break;
		}
		map[i].t = t[i];
	}
}

void esb::Sprite::setX(int xPos) {
	Translate(xPos - x, 0);
	x = xPos;
}

void esb::Sprite::setY(int yPos) {
	Translate(0, yPos - y);
	y = yPos;
}

int esb::Sprite::getX() {
	return x;
}

int esb::Sprite::getY() {
	return y;
}