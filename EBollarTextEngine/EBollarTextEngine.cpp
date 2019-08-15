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

void esb::EBollarTextEngine::cls() {
	static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE); // gets rid of cursor

	CONSOLE_SCREEN_BUFFER_INFO csbi;
	COORD topLeft = { 0, 0 };

	std::cout.flush();

	if (!GetConsoleScreenBufferInfo(hOut, &csbi)) {
		abort();
	}
	DWORD length = csbi.dwSize.X * csbi.dwSize.Y;

	DWORD written;

	FillConsoleOutputCharacter(hOut, TEXT(' '), length, topLeft, &written);
	FillConsoleOutputAttribute(hOut, csbi.wAttributes, length, topLeft, &written);
	SetConsoleCursorPosition(hOut, topLeft);
}

void esb::EBollarTextEngine::setCursorPosition(int x, int y) // puts cursor at point on screen - allows for double buffering
{
	static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	std::cout.flush();
	COORD coord = { (SHORT)x, (SHORT)y };
	SetConsoleCursorPosition(hOut, coord);
}

void esb::EBollarTextEngine::ShowConsoleCursor(bool ysno)
{
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_CURSOR_INFO     cursorInfo;

	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = ysno; // sets the cursor visibility
	SetConsoleCursorInfo(out, &cursorInfo);
}

void esb::EBollarTextEngine::setConsoleColour(WORD* Attributes, DWORD Color) // sets color of next print
{
	CONSOLE_SCREEN_BUFFER_INFO Info;
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(hStdout, &Info);
	*Attributes = Info.wAttributes;
	SetConsoleTextAttribute(hStdout, Color);
}

void esb::EBollarTextEngine::ResetConsoleColour(WORD Attributes) // sets color to blank
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Attributes);
}

void esb::EBollarTextEngine::setRefreshRate(int milliseconds) { // sets delay between render updates
	rRate = milliseconds;
}

int esb::EBollarTextEngine::FindSpriteIndex(std::string name, int id) { // returns position of sprite in vector with given name and id (used if multiple instances have given name)
	for (int i = 0; i < s.sprites.size(); i++) {
		if (s.sprites.at(i).name == name) {
			if (id == -1) {
				return i;
			}
			else {
				if (s.sprites.at(i).id == id) {
					return i;
				}
			}
		}
	}

	Stop();
	std::cout << "Error, sprite '" + name + "' with possible id: " + std::to_string(id) + " could not be found."; // temporary error msg
	return 0;
}

void esb::EBollarTextEngine::FillScene(color c) {
	s.arr2D.clear(); // clear arr2D because we are going to replace all values
	s.row.clear(); // clears the filler row that arr2D uses to render frame
	for (int height = 0; height < s.h; height++) {
		for (int width = 0; width < s.w; width++) { // fills each index in arr2D with color c
			Pixel currPix;
			currPix.c = c;
			s.row.push_back(currPix); // 1D array x is current row
		}
		s.arr2D.push_back(s.row); // add current row to arr2D
		s.row.clear(); // clear current row because we are now at x = 0
	}
}

bool esb::EBollarTextEngine::OnKey(direction key) {
	switch (key) {
		case RIGHT:
			if (GetKeyState(VK_RIGHT) & 0x8000 && canPress) // this function just returns if designated key is pressed or not
			{
				return true;
				canPress = false; // can only recieve one input per frame so we dont make it go from 0 to a bajillion in one frame
			}
			else {
				return false;
			}
		case LEFT:
			if (GetKeyState(VK_LEFT) & 0x8000 && canPress)
			{
				return true;
				canPress = false;
			}
			else {
				return false;
			}
		case UP:
			if (GetKeyState(VK_UP) & 0x8000 && canPress)
			{
				return true;
				canPress = false;
			}
			else {
				return false;
			}
		case DOWN:
			if (GetKeyState(VK_DOWN) & 0x8000 && canPress)
			{
				return true;
				canPress = false;
			}
			else {
				return false;
			}
	}
}

std::vector<esb::Pixel> esb::EBollarTextEngine::CompareFrames(std::vector<std::vector<esb::Pixel> >& prevFrame, std::vector<std::vector<esb::Pixel> >& nextFrame) { // jus t takes previous frame and suspected next frame, and returns the changes
	std::vector<Pixel> diffs; // diffs between prev and next frame

	for (int yPos = 0; yPos < s.h; yPos++) {
		for (int xPos = 0; xPos < s.w; xPos++) {
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

void esb::EBollarTextEngine::PrintScene() {
	FillScene(baseColor); // easy background and reset (although not most efficient this is windows console so i don't think its a huge problem...)
	SpriteCollate(); // put them on de top uh de ting
	setCursorPosition(0, 0);

	std::vector<Pixel> diffs = CompareFrames(s.prevFrame, s.arr2D);

	for (int i = 0; i < diffs.size(); i++) {
		setCursorPosition(diffs.at(i).x, diffs.at(i).y);

		setConsoleColour(&Attributes, BACKGROUND_INTENSITY | c(diffs.at(i).c)); // this just sets the color of the current pixel that it's printing to the console's color scheme
		std::cout << diffs.at(i).t; // woo hoo it prints a thing

		ResetConsoleColour(Attributes); // this is only useful once we've finished printing a frame so the things don't cross over to next frame
	}

	if (!RUNNING) { // if game loop ends put cursor just below entire frame so it gets out the way or if u wanna print additional messages
		setCursorPosition(0, s.h);
	}

	std::cout.flush();
	s.prevFrame = s.arr2D; // now do it all again bub
}

DWORD esb::EBollarTextEngine::c(color c) { // makes things easier i think
	switch (c) {
	case BLUE :
		return BACKGROUND_BLUE;
	case RED :
		return BACKGROUND_RED;
	case GREEN :
		return BACKGROUND_GREEN;
	case YELLOW :
		return (BACKGROUND_RED | BACKGROUND_GREEN);
	case PINK :
		return (BACKGROUND_RED | BACKGROUND_BLUE);
	case LIGHTBLUE:
		return (BACKGROUND_GREEN | BACKGROUND_BLUE);
	case BLACK:
		return (BACKGROUND_INTENSITY);
	case WHITE :
		return (BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
	}
}

void esb::EBollarTextEngine::MakeSpriteRect(int x, int y, int w, int h, color c, std::string name, int id) { // makes a rect sprite (rectangle is made down and right from 0, 0 so position of rectangle is always in relation to top-left)
	Sprite newSprite;
	x--; y--; // adjusts so that 0, 0 is correct
	newSprite.x = x;
	newSprite.y = y;
	w = w * 2; // ok so characters when printed are in a rectangle with a ratio close to 2 height to 1 width so we can "fix" this by just printing evey pixel twice on the x axis so its proportionally correct kinda idk easy fix ig
	
	newSprite.name = name;
	if (id != -1) {
		newSprite.id = id;
	}

	for (int xOffset = 0; xOffset < w; xOffset++) { // sets width and height
		for (int yOffset = 0; yOffset < h; yOffset++) {
			Pixel curr;
			curr.x = x + xOffset;
			curr.y = y + yOffset;
			curr.c = c;
			newSprite.map.push_back(curr); // adds that new pixel to the spritemap for newSprite
		}
	}

	s.sprites.push_back(newSprite); // new rectangle sprite auto-added to scene's sprite vec
}

void esb::EBollarTextEngine::MakeSprite(int x, int y, std::vector<esb::Pixel> m, std::string name, int id) {
	Sprite newSprite;
	x--; y--; // adjusts for 0, 0 to be topleft
	newSprite.name = name;
	if (id != -1) {
		newSprite.id = id;
	}

	for (int q = 0; q < m.size(); q++) { // sets width and height
		newSprite.map.push_back(m[q]);
		newSprite.map[q].x += x;
		newSprite.map[q].y += y;
	}

	s.sprites.push_back(newSprite);
}

void esb::EBollarTextEngine::SpriteCollate() { // overlay all sprite maps onto scene arr2D
	if (s.sprites.size() > 0) {
		for (int currSprite = 0; currSprite < s.sprites.size(); currSprite++) { // current sprite
			for (int currPix = 0; currPix < s.sprites.at(currSprite).map.size(); currPix++) { // current pixel in current sprite's spritemap
				std::vector<Pixel> currSpriteMap = s.sprites.at(currSprite).map; // temp container for currSprite just so it doesn't like tooooooo messy even though it kinda does lol
				s.arr2D.at(currSpriteMap[currPix].y).at(currSpriteMap[currPix].x).c = currSpriteMap[currPix].c; // sets the color of x,y coordinate of pixel in spritemap to scene's new, (not yet loaded) frame with text and color
				s.arr2D.at(currSpriteMap[currPix].y).at(currSpriteMap[currPix].x).t = currSpriteMap[currPix].t;
				s.arr2D.at(currSpriteMap[currPix].y).at(currSpriteMap[currPix].x).textColor = currSpriteMap[currPix].textColor;
			}
		}
	}
}

void esb::EBollarTextEngine::DelSprite(std::string name, int id) {
	s.sprites.erase(s.sprites.begin() + FindSpriteIndex(name, id));
}

void esb::EBollarTextEngine::ClearSprites() {
	s.sprites.clear();
}

void esb::EBollarTextEngine::Render() {
	PrintScene();
	std::this_thread::sleep_for(std::chrono::milliseconds(rRate)); // adjustable delay
	canPress = true;

	/*
	if (!RUNNING) { // if gameloop ends, stop rendering
		Stop();
	
	*/
}

void esb::EBollarTextEngine::ConstructScene(int width, int height) {
	s.w = width * 2; // gotta stretch it otherwise it looks to skinny cause of the natural rectangular shape of a ' '
	s.h = height;
	
	for (int y = 0; y < s.h; y++) { // since there is no prev frame to double buffer through, i fill the previous frame with a blank frame on instantiation - easy fix although probably not most efficient
		for (int x = 0; x < s.w; x++) {
			Pixel p;
			p.c = baseColor;
			s.filler.push_back(p);
		}
		s.prevFrame.push_back(s.filler);
		s.filler.clear();
	}

	esb::EBollarTextEngine::ClearSprites(); // clear all the things
	s.arr2D.clear(); // clears upcoming frame just for instantiation
	s.row.clear();

	for (int x = 0; x < s.w; x++) { // fill new frame with solid color just to set up things
		for (int y = 0; y < s.h; y++) {
			Pixel p;
			p.c = baseColor;
			s.row.push_back(p);
		}
		s.arr2D.push_back(s.row);
		s.row.clear();
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

void esb::EBollarTextEngine::MoveSprite(int xC, int yC, std::string name, int id) {
	Sprite curr = s.sprites.at(FindSpriteIndex(name, id));

	while (xC != 0 || yC != 0) {

		bool rL = false; // sprite - am i restricted from moving in a direction
		bool rR = false;
		bool rU = false;
		bool rD = false;

		for (int i = 0; i < curr.map.size(); i++) { // checks if any of sprite's pixels are directly next to resolution border (out of scene's 2d array index) and restricting movement in that dir.
			if (curr.map.at(i).x - 1 < 0) {
				rL = true;
			}
			else if (curr.map.at(i).x + 1 > s.w - 1) {
				rR = true;
			}
			if (curr.map.at(i).y - 1 < 0) {
				rU = true;
			}
			else if (curr.map.at(i).y + 1 > s.h - 1) {
				rD = true;
			}
		}

		for (int i = 0; i < curr.map.size(); i++) { // adjusts each pixel in spritemap according to movement command by 1 or -1, then it rechecks restrictions (above this for loop)
			if (xC > 0 && !rR) {
				s.arr2D.at(curr.map.at(i).y).at(curr.map.at(i).x).c = baseColor; // moving right
				s.arr2D.at(curr.map.at(i).y).at(curr.map.at(i).x).t = ' ';
				curr.map.at(i).x += 1;
				curr.x += 1;
				std::cout << curr.x;
				xC--;
			}
			else if (xC < 0 && !rL) {
				s.arr2D.at(curr.map.at(i).y).at(curr.map.at(i).x).c = baseColor; // moving left
				s.arr2D.at(curr.map.at(i).y).at(curr.map.at(i).x).t = ' ';
				curr.map.at(i).x += -1;
				curr.x -= 1;
				std::cout << curr.x;
				xC++;
			}
			if (yC < 0 && !rU) {
				s.arr2D.at(curr.map.at(i).y).at(curr.map.at(i).x).c = baseColor; // moving up
				s.arr2D.at(curr.map.at(i).y).at(curr.map.at(i).x).t = ' ';
				curr.map.at(i).y += -1;
				curr.y -= 1;
				std::cout << curr.y;
				yC++;
			}
			else if (yC > 0 && !rD) {
				s.arr2D.at(curr.map.at(i).y).at(curr.map.at(i).x).c = baseColor; // moving down
				s.arr2D.at(curr.map.at(i).y).at(curr.map.at(i).x).t = ' ';
				curr.map.at(i).y += 1;
				curr.y += 1;
				std::cout << curr.y;
				yC--;
			}
		}
	}

	s.sprites[FindSpriteIndex(name, id)] = curr;
}

void esb::EBollarTextEngine::Stop() {
	RUNNING = false;
}

std::string esb::EBollarTextEngine::CheckSpriteCollide(std::string name, int id) {
	Sprite curr = s.sprites.at(FindSpriteIndex(name, id));
	
	for (int testSprite = 0; testSprite < s.sprites.size(); testSprite++) { // each sprite
		for (int testPix = 0; testPix < s.sprites.at(testSprite).map.size(); testPix++) { // each pixel in spritemap of sprite we are testing for
			if (testSprite != FindSpriteIndex(name, id)) { // make sure not my sprite
				for (int currPix = 0; currPix < curr.map.size(); currPix++) { // each pixel in my sprite's spritemap
					std::vector<esb::Pixel> mySpritemap = curr.map;
					std::vector<esb::Pixel> testingSpritemap = s.sprites.at(testSprite).map;

					if (mySpritemap.at(currPix).x == testingSpritemap.at(testPix).x && mySpritemap.at(currPix).y == testingSpritemap.at(testPix).y) {
						return s.sprites.at(testSprite).name;
					}
				}
			}
		}
	}

	return ""; // return nothing if no collision detected
};

int esb::EBollarTextEngine::getPosX(std::string name, int id) {
	return s.sprites.at(FindSpriteIndex(name, id)).x;
}

int esb::EBollarTextEngine::getPosY(std::string name, int id) {
	return s.sprites.at(FindSpriteIndex(name, id)).y;
}

void esb::EBollarTextEngine::AddSprite(esb::Sprite a) {
	s.sprites.push_back(a);
}

void esb::EBollarTextEngine::SetBackground(esb::color c) {
	baseColor = c;
}

void esb::EBollarTextEngine::MakeText(int x, int y, color c, std::string string, std::string name, int id) {
	int div; // the middle of the string kinda
	if (string.size() % 2 == 0) {
		div = string.size() / 2;
	}
	else {
		div = round(string.size() / 2.0 - 0.5);
	}

	MakeSpriteRect(x, y, 1, string.size() - div, c, name, id);
	Sprite curr = s.sprites.at(FindSpriteIndex(name, id));

	for (int i = 0; i < string.size(); i++) { // adds all the text in
		curr.map.at(i).t = string.at(i);
		curr.map.at(i).textColor = c;
	}

	DelSprite(name, id);
	s.sprites.push_back(curr);
}

void esb::EBollarTextEngine::setText(std::string string, std::string name, int id) {
	Sprite curr = s.sprites.at(FindSpriteIndex(name, id));
	DelSprite(name);
	MakeText(curr.x + 1, curr.y + 1, curr.map.at(0).c, string, name, id);
}


void esb::EBollarTextEngine::setX(int x, std::string name, int id) {
	x = x * 2;
	MoveSprite(x - s.sprites.at(FindSpriteIndex(name, id)).x, 0, name, id);
}

void esb::EBollarTextEngine::setY(int y, std::string name, int id) {
	MoveSprite(0, y - s.sprites.at(FindSpriteIndex(name, id)).x, name, id);
}
// not working rip


