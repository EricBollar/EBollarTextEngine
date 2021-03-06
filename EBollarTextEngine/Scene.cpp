//
// The EBollarTextEngine is a 2D Game Engine that uses the Windows Console
//        - Eric
//

#include "EBollarTextEngine.h"
#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <thread>
#include <chrono>
#include <Windows.h>

esb::Sprite* esb::Scene::FindSprite(std::string name, int id) { // returns position of sprite in vector with given name and id (used if multiple instances have given name)
	for (int i = 0; i < sprites.size(); i++) {
		if (sprites.at(i)->getName() == name) {
			if (id == -1) {
				esb::Sprite* ptr = sprites.at(i);
				return ptr;
			}
			else {
				if (sprites.at(i)->getID() == id) {
					esb::Sprite* ptr = sprites.at(i);
					return ptr;
				}
			}
		}
	}

	std::cout << "no sprite found rip lol";
}

void esb::Scene::FillScene(color c) {
	arr2D.clear(); // clear arr2D because we are going to replace all values
	row.clear(); // clears the filler row that arr2D uses to render frame
	for (int height = 0; height < h; height++) {
		for (int width = 0; width < w; width++) { // fills each index in arr2D with color c
			Pixel currPix;
			currPix.c = c;
			row.push_back(currPix); // 1D array x is current row
		}
		arr2D.push_back(row); // add current row to arr2D
		row.clear(); // clear current row because we are now at x = 0
	}
}

std::vector<esb::Pixel> esb::Scene::CompareFrames(std::vector<std::vector<esb::Pixel> >& prevFrame, std::vector<std::vector<esb::Pixel> >& nextFrame) { // just takes previous frame and suspected next frame, and returns the changes
	std::vector<Pixel> diffs; // diffs between prev and next frame

	for (int yPos = 0; yPos < h; yPos++) {
		for (int xPos = 0; xPos < w; xPos++) {
			if (prevFrame.at(yPos).at(xPos).c != nextFrame.at(yPos).at(xPos).c || prevFrame.at(yPos).at(xPos).t != nextFrame.at(yPos).at(xPos).t) { // both frames are same dimensions so we can use loop for both
				Pixel curr; // if the pixel is different between prev and new frame, add its coordinates and new color to a new vec2D and push it to diffs vec
				curr.x = xPos;
				curr.y = yPos;
				curr.c = nextFrame.at(yPos).at(xPos).c; // want to set the color and text to the next frame, not previous one
				curr.t = nextFrame.at(yPos).at(xPos).t;
				curr.textColor = nextFrame.at(yPos).at(xPos).textColor;
				diffs.push_back(curr);
			}
		}
	}

	prevFrame = arr2D;
	return diffs;
};

void esb::Scene::clearDiffs() {
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			Pixel p;
			p.x = -1;
			p.y = -1;
			p.c = esb::RED;
			p.t = "~~51876267812873612987638971269837!@#$%^&*()_+";

			prevFrame[y][x] = p;
		}
	}
}

void esb::Scene::Process() {
	FillScene(baseColor); // easy background and reset (although not most efficient this is windows console so i don't think its a huge problem...)
	SpriteCollate(); // put them on de top uh de ting
}

esb::Sprite* esb::Scene::MakeSpriteRect(int x, int y, int width, int height, color c, std::string name, int id) { // makes a rect sprite (rectangle is made down and right from 0, 0 so position of rectangle is always in relation to top-left)
	Sprite* newSprite = new Sprite();
	x--; y--; // adjusts so that 0, 0 is correct
	newSprite->setName(name);
	newSprite->setID(id);
	newSprite->setW(w);
	newSprite->setH(h);
	newSprite->setX(x);
	newSprite->setY(y);
	int newInd = sprites.size();
	newSprite->setIndex(newInd);

	std::vector<esb::Pixel> map;
	for (int xOffset = 0; xOffset < width; xOffset++) { // sets width and height
		for (int yOffset = 0; yOffset < height; yOffset++) {
			Pixel curr;
			curr.x = xOffset;
			curr.y = yOffset;
			curr.c = c;
			map.push_back(curr); // adds that new pixel to the spritemap for newSprite
		}
	}

	newSprite->setMap(map);

	sprites.push_back(newSprite); // new rectangle sprite auto-added to scene's sprite vec

	return sprites.at(newInd);
}

esb::Sprite* esb::Scene::MakeSprite(int x, int y, std::vector<esb::Pixel> m, std::string name, int id) {
	Sprite* newSprite = new Sprite();
	x--; y--; // adjusts so that 0, 0 is correct
	newSprite->setName(name);
	newSprite->setID(id);
	newSprite->setW(w);
	newSprite->setH(h);
	newSprite->setMap(m);
	newSprite->setX(x);
	newSprite->setY(y);

	int newInd = sprites.size();
	sprites.push_back(newSprite);
	newSprite->setIndex(newInd);

	return sprites.at(newInd);
}

void esb::Scene::SpriteCollate() { // overlay all sprite maps onto scene arr2D
	if (sprites.size() > 0) {
		for (int currSprite = 0; currSprite < sprites.size(); currSprite++) { // current sprite
			for (int currPix = 0; currPix < sprites.at(currSprite)->getMap().size(); currPix++) { // current pixel in current sprite's spritemap
				std::vector<Pixel> currSpriteMap = sprites.at(currSprite)->getMap(); // temp container for currSprite just so it doesn't like tooooooo messy even though it kinda does lol
				arr2D.at(currSpriteMap[currPix].y + sprites.at(currSprite)->getY()).at(currSpriteMap[currPix].x + sprites.at(currSprite)->getX()).c = currSpriteMap[currPix].c; // sets the color of x,y coordinate of pixel in spritemap to scene's new, (not yet loaded) frame with text and color
				arr2D.at(currSpriteMap[currPix].y + sprites.at(currSprite)->getY()).at(currSpriteMap[currPix].x + sprites.at(currSprite)->getX()).t = currSpriteMap[currPix].t;
				arr2D.at(currSpriteMap[currPix].y + sprites.at(currSprite)->getY()).at(currSpriteMap[currPix].x + sprites.at(currSprite)->getX()).textColor = currSpriteMap[currPix].textColor;
				if (currSpriteMap[currPix].t != "" && currPix < sprites.at(currSprite)->getMap().size() - 1) {
					int count = 0;
					int textPix = currPix;
					while (currSpriteMap[currPix + 1].t != "") {
						arr2D.at(currSpriteMap[textPix].y + sprites.at(textPix)->getY()).at(currSpriteMap[textPix].x + sprites.at(currSprite)->getX()).t += currSpriteMap[currPix + 1].t;
						currPix++;
						count++;
					}
					if (count % 2 != 0) {
						arr2D.at(currSpriteMap[textPix].y + sprites.at(currSprite)->getY()).at(currSpriteMap[textPix].x + sprites.at(currSprite)->getX()).t += ' ';
					}
				}
			}
		}
	}
}

void esb::Scene::DelSprite(esb::Sprite s) {
	sprites.erase(sprites.begin() + s.getIndex());
}

void esb::Scene::ClearSprites() {
	sprites.clear();
}

void esb::Scene::ConstructScene(int width, int height, esb::color backgroundColor) {
	w = width; // gotta stretch it otherwise it looks to skinny cause of the natural rectangular shape of a ' '
	h = height;
	
	for (int y = 0; y < h; y++) { // since there is no prev frame to double buffer through, i fill the previous frame with a blank frame on instantiation - easy fix although probably not most efficient
		for (int x = 0; x < w; x++) {
			Pixel p;
			p.c = baseColor;
			filler.push_back(p);
		}
		prevFrame.push_back(filler);
		filler.clear();
	}

	esb::Scene::ClearSprites(); // clear all the things
	arr2D.clear(); // clears upcoming frame just for instantiation
	row.clear();

	for (int x = 0; x < w; x++) { // fill new frame with solid color just to set up things
		for (int y = 0; y < h; y++) {
			Pixel p;
			p.c = baseColor;
			row.push_back(p);
		}
		arr2D.push_back(row);
		row.clear();
	}
	baseColor = backgroundColor;
}

/* This isn't really useful i don't think
void esb::EBollarTextEngine::CheckSpriteCOO() { // if user adjusted the x or y on sprite engine needs to adjust the sprites map to those new coordinates
	for (int i = 0; i < s.sprites.size(); i++) { // each sprite
		for (int j = 0; j < s.sprites.at(i).map.size(); j++) { // each pixel in spritemap
			Sprite curr = s.sprites.at(i); // adjusts the x and y of each pixel in spritemap based on that of whole sprite's x and y coordinate -> easy teleport instead of blinding moving in a dir.
			curr.map.at(j).x += curr.x;
			curr.map.at(j).y += curr.y;
		}
	}
}
*/

esb::color esb::Scene::getBackgroundColor() {
	return baseColor;
}

bool esb::Scene::CheckSpriteCollide(esb::Sprite* sprite1, esb::Sprite* sprite2) {
	for (int i = 0; i < sprite1->getMap().size(); i++) {
		esb::Pixel s1Pix = sprite1->getMap().at(i);
		for (int q = 0; q < sprite2->getMap().size(); q++) {
			esb::Pixel s2Pix = sprite2->getMap().at(q);
			if (s1Pix.x + sprite1->getX() == s2Pix.x + sprite2->getX()) {
				if (s1Pix.y + sprite1->getY() == s2Pix.y + sprite2->getY()) {
					return true;
				}
			}
		}
	}
	return false;

	/* this is broken :(
	for (int testSprite = 0; testSprite < sprites.size(); testSprite++) { // each sprite
		for (int testPix = 0; testPix < sprites.at(testSprite)->getMap().size(); testPix++) { // each pixel in spritemap of sprite we are testing for
			if (*sprites.at(testSprite) != s) { // make sure not my sprite
				for (int currPix = 0; currPix < s.getMap().size(); currPix++) { // each pixel in my sprite's spritemap
					std::vector<esb::Pixel> mySpritemap = s.getMap();
					std::vector<esb::Pixel> testingSpritemap = sprites.at(testSprite)->getMap();

					if (mySpritemap.at(currPix).x + s.getX() == testingSpritemap.at(testPix).x + sprites.at(testSprite)->getX() && mySpritemap.at(currPix).y + s.getY() == testingSpritemap.at(testPix).y + sprites.at(testSprite)->getY()) {
						return sprites.at(testSprite)->getName();
					}
				}
			}
		}
	}*/
};

void esb::Scene::AddSprite(esb::Sprite a) {
	esb::Sprite* ptr = &a;
	sprites.push_back(ptr);
}

esb::Sprite* esb::Scene::MakeText(int x, int y, color c, std::string string, std::string name, int id) {
	int w;
	std::string s = string;
	
	if (string.size() % 2 != 0) {
		s += " ";
	}


	esb::Sprite* ptr = MakeSpriteRect(x, y, s.size()/2, 1, c, name, id);
	ptr->setText(s);
	return ptr;
}

int esb::Scene::getW() {
	return this->w;
}

int esb::Scene::getH() {
	return this->h;
}

std::vector<esb::Pixel> esb::Scene::getDiffs() {
	return CompareFrames(prevFrame, arr2D);
}

std::vector<std::vector<esb::Pixel> > esb::Scene::getFrame() {
	return arr2D;
}

/*
void esb::Scene::moveSprite(esb::Sprite* s, int xChange, int yChange) {
	esb::Sprite temp = *s;

	DelSprite(FindSprite(temp.getName(), temp.getID()));
	temp.Translate(xChange, yChange);

	*s = temp;
	AddSprite(temp);
}*/
