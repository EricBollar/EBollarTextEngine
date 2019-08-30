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

esb::Sprite esb::Scene::FindSprite(std::string name, int id) { // returns position of sprite in vector with given name and id (used if multiple instances have given name)
	for (int i = 0; i < sprites.size(); i++) {
		if (sprites.at(i).getName() == name) {
			if (id == -1) {
				return sprites.at(i);
			}
			else {
				if (sprites.at(i).getID() == id) {
					return sprites.at(i);
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

	return diffs;
};

void esb::Scene::Process() {
	FillScene(baseColor); // easy background and reset (although not most efficient this is windows console so i don't think its a huge problem...)
	SpriteCollate(); // put them on de top uh de ting
}

void esb::Scene::MakeSpriteRect(int x, int y, int w, int h, color c, std::string name, int id) { // makes a rect sprite (rectangle is made down and right from 0, 0 so position of rectangle is always in relation to top-left)
	Sprite newSprite;
	x--; y--; // adjusts so that 0, 0 is correct
	newSprite.setName(name);
	newSprite.setID(id);
	newSprite.setW(w);
	newSprite.setH(h);

	std::vector<esb::Pixel> map;
	for (int xOffset = 0; xOffset < w; xOffset++) { // sets width and height
		for (int yOffset = 0; yOffset < h; yOffset++) {
			Pixel curr;
			curr.x = x + xOffset;
			curr.y = y + yOffset;
			curr.c = c;
			map.push_back(curr); // adds that new pixel to the spritemap for newSprite
		}
	}

	int midX = 0;
	int midY = 0;

	if (w % 2 == 0) {
		midX = w / 2;
	}
	else {
		midX = round(w / 2.0 - 0.5);
	}

	if (h % 2 == 0) {
		midY = h / 2;
	}
	else {
		midY = round(h / 2.0 - 0.5);
	}

	newSprite.setX(midX);
	newSprite.setY(midY);
	newSprite.setMap(map);

	int newInd = sprites.size();
	sprites.push_back(newSprite); // new rectangle sprite auto-added to scene's sprite vec
	newSprite.setIndex(newInd);
}

void esb::Scene::MakeSprite(int x, int y, std::vector<esb::Pixel> m, std::string name, int id) {
	Sprite newSprite;
	x--; y--; // adjusts so that 0, 0 is correct
	newSprite.setName(name);
	newSprite.setID(id);
	newSprite.setW(w);
	newSprite.setH(h);
	newSprite.setMap(m);

	int newInd = sprites.size();
	sprites.push_back(newSprite);
	newSprite.setIndex(newInd);
}

void esb::Scene::SpriteCollate() { // overlay all sprite maps onto scene arr2D
	if (sprites.size() > 0) {
		for (int currSprite = 0; currSprite < sprites.size(); currSprite++) { // current sprite
			for (int currPix = 0; currPix < sprites.at(currSprite).getMap().size(); currPix++) { // current pixel in current sprite's spritemap
				std::vector<Pixel> currSpriteMap = sprites.at(currSprite).getMap(); // temp container for currSprite just so it doesn't like tooooooo messy even though it kinda does lol
				arr2D.at(currSpriteMap[currPix].y).at(currSpriteMap[currPix].x).c = currSpriteMap[currPix].c; // sets the color of x,y coordinate of pixel in spritemap to scene's new, (not yet loaded) frame with text and color
				arr2D.at(currSpriteMap[currPix].y).at(currSpriteMap[currPix].x).t = currSpriteMap[currPix].t;
				arr2D.at(currSpriteMap[currPix].y).at(currSpriteMap[currPix].x).textColor = currSpriteMap[currPix].textColor;
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

void esb::Scene::ConstructScene(int width, int height) {
	w = width * 2; // gotta stretch it otherwise it looks to skinny cause of the natural rectangular shape of a ' '
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

	RUNNING = true; // game is now going
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

void esb::Scene::Stop() {
	RUNNING = false;
}

esb::Sprite esb::Scene::CheckSpriteCollide(esb::Sprite s) {
	for (int testSprite = 0; testSprite < sprites.size(); testSprite++) { // each sprite
		for (int testPix = 0; testPix < sprites.at(testSprite).getMap().size(); testPix++) { // each pixel in spritemap of sprite we are testing for
			if (testSprite != s.getID()) { // make sure not my sprite
				for (int currPix = 0; currPix < s.getMap().size(); currPix++) { // each pixel in my sprite's spritemap
					std::vector<esb::Pixel> mySpritemap = s.getMap();
					std::vector<esb::Pixel> testingSpritemap = sprites.at(testSprite).getMap();

					if (mySpritemap.at(currPix).x == testingSpritemap.at(testPix).x && mySpritemap.at(currPix).y == testingSpritemap.at(testPix).y) {
						return sprites.at(testSprite);
					}
				}
			}
		}
	}
};

void esb::Scene::AddSprite(esb::Sprite a) {
	sprites.push_back(a);
}

void esb::Scene::SetBackground(esb::color c) {
	baseColor = c;
}

void esb::Scene::MakeText(int x, int y, color c, std::string string, std::string name, int id) {
	int div; // the middle of the string kinda
	if (string.size() % 2 == 0) {
		div = string.size() / 2;
	}
	else {
		div = round(string.size() / 2.0 - 0.5);
	}

	MakeSpriteRect(x, y, 1, string.size() - div, c, name, id);
	Sprite curr = FindSprite(name, id);
	curr.setText(string);
	
	sprites.push_back(curr);
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


