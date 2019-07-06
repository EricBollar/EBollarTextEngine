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

void esb::EBollarTextEngine::setCursorPosition(int x, int y) // puts cursor at point on screen - allows double buffering
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
	cursorInfo.bVisible = ysno; // set the cursor visibility
	SetConsoleCursorInfo(out, &cursorInfo);
}

void esb::EBollarTextEngine::setConsoleColour(WORD* Attributes, DWORD Colour)
{
	CONSOLE_SCREEN_BUFFER_INFO Info;
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(hStdout, &Info);
	*Attributes = Info.wAttributes;
	SetConsoleTextAttribute(hStdout, Colour);
}

void esb::EBollarTextEngine::ResetConsoleColour(WORD Attributes)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Attributes);
}

void esb::EBollarTextEngine::RefreshRate(int milliseconds) {
	rRate = milliseconds;
}

int esb::EBollarTextEngine::FindSpriteWithName(std::string name) {
	for (int i = 0; i < s.sprites.size(); i++) {
		if (s.sprites.at(i).name == name) {
			return i;
		}
	}
}

void esb::EBollarTextEngine::FillScene(color c) {
	s.arr2D.clear(); // clear arr2D because we are going to replace all values
	s.x.clear();
	for (int y = 0; y < s.h; y++) {
		for (int x = 0; x < s.w; x++) { // fills each index in arr2D with color c
			s.x.push_back(c); // 1D array x is current row
		}
		s.arr2D.push_back(s.x); // add current row to arr2D
		s.x.clear(); // clear current row because we are now at x = 0
	}
}

bool esb::EBollarTextEngine::OnKey(direction key) {
	switch (key) {
		case RIGHT:
			if (GetKeyState(VK_RIGHT) & 0x8000 && canMove) // this function just returns if designated key is pressed or not
			{
				return true;
				canMove = false; // can only recieve one input per frame so we dont make it go from 0 to a bajillion in one frame
			}
			else {
				return false;
			}
		case LEFT:
			if (GetKeyState(VK_LEFT) & 0x8000 && canMove)
			{
				return true;
				canMove = false;
			}
			else {
				return false;
			}
		case UP:
			if (GetKeyState(VK_UP) & 0x8000 && canMove)
			{
				return true;
				canMove = false;
			}
			else {
				return false;
			}
		case DOWN:
			if (GetKeyState(VK_DOWN) & 0x8000 && canMove)
			{
				return true;
				canMove = false;
			}
			else {
				return false;
			}
	}
}

std::vector<vec2D> esb::EBollarTextEngine::CompareFrames(std::vector<std::vector<color> >& frame1, std::vector<std::vector<color> >& frame2) {
	std::vector<vec2D> diffs;

	for (int y = 0; y < frame1.size(); y++) {
		for (int x = 0; x < frame1.at(y).size(); x++) {
			if (frame1.at(y).at(x) != frame2.at(y).at(x)) { // both frames are same dimensions so we can use for loops for both
				vec2D curr; // if the pixel is different between prev and new frame, add its coordinates and new color to a new vec2D and push it to diffs vec
				curr.x = x;
				curr.y = y;
				curr.c = frame2.at(y).at(x);
				diffs.push_back(curr);
			}
		}
	}

	return diffs;
};

void esb::EBollarTextEngine::PrintScene() {
	FillScene(baseColor);
	SpriteCollate();

	std::vector<vec2D> diffs = CompareFrames(s.prevFrame, s.arr2D);

	for (int i = 0; i < diffs.size(); i++) {
		setCursorPosition(diffs.at(i).x, diffs.at(i).y);
		setConsoleColour(&Attributes, BACKGROUND_INTENSITY | c(diffs.at(i).c));
		std::cout << " ";
		ResetConsoleColour(Attributes);
	}

	std::cout.flush();
	s.prevFrame = s.arr2D;
}

DWORD esb::EBollarTextEngine::c(color c) {
	switch (c) {
	case BLUE :
		return BACKGROUND_BLUE;
	case RED :
		return BACKGROUND_RED;
	case GREEN :
		return BACKGROUND_GREEN;
	case NOCOL :
		return BACKGROUND_INTENSITY, BACKGROUND_BLUE, BACKGROUND_GREEN, BACKGROUND_RED;
	}
}

void esb::EBollarTextEngine::PaintPix(int x, int y, color c) {
	x--; y--;
	s.arr2D.at(y).at(x) = c;
}

void esb::EBollarTextEngine::MakeSpriteRect(int x, int y, int w, int h, color c, std::string name) {
	Sprite a;
	x--; y--;
	a.x = x;
	a.y = y;
	h = h * 2;
	
	a.name = name;

	for (int i = 0; i < h; i++) { // sets width and height
		for (int j = 0; j < w; j++) {
			vec2D curr;
			curr.x = x + i;
			curr.y = y + j;
			curr.c = c;
			a.map.push_back(curr);
		}
	}

	s.sprites.push_back(a);
}

void esb::EBollarTextEngine::MakeSprite(int x, int y, std::vector<vec2D> m, std::string name) {
	Sprite a;
	x--; y--;
	a.name = name;

	for (int q = 0; q < m.size(); q++) { // sets width and height
		a.map.push_back(m[q]);
		a.map[q].x += x;
		a.map[q].y += y;
	}

	s.sprites.push_back(a);
}

void esb::EBollarTextEngine::SpriteCollate() { // overlay all sprite maps onto scene arr2D
	if (s.sprites.size() > 0) {
		for (int sprts = 0; sprts < s.sprites.size(); sprts++) { // current sprite
			for (int i = 0; i < s.sprites.at(sprts).map.size(); i++) { // current vec2D
				Sprite spr = s.sprites.at(sprts);
				s.arr2D.at(spr.map[i].y).at(spr.map[i].x) = spr.map[i].c;
			}
		}
	}
}

void esb::EBollarTextEngine::DelSprite(std::string name) {
	s.sprites.erase(s.sprites.begin() + FindSpriteWithName(name));
}

void esb::EBollarTextEngine::ClearSprites() {
	s.sprites.clear();
}

void esb::EBollarTextEngine::GameLoop() {
	PrintScene();
	std::this_thread::sleep_for(std::chrono::milliseconds(rRate));
	canMove = true;
	gameLoop = m_Running;
}

void esb::EBollarTextEngine::ConstructScene(int width, int height) {
	s.w = width;
	s.h = height;
	
	for (int q = 0; q < s.h; q++) { // since there is no prev frame to double buffer through, i fill a blank frame on instantiation
		for (int i = 0; i < s.w; i++) {
			s.filler.push_back(NOCOL);
		}
		s.prevFrame.push_back(s.filler);
		s.filler.clear();
	}

	esb::EBollarTextEngine::ClearSprites(); // clear all the things
	s.arr2D.clear();
	s.x.clear();
	for (int q = 0; q < s.w; q++) { // fill new frame with solid color just to set up things
		for (int i = 0; i < s.h; i++) {
			s.x.push_back(baseColor);
		}
		s.arr2D.push_back(s.x);
		s.x.clear();
	}

	m_Running = true; // game is now going
	gameLoop = m_Running;
}


void esb::EBollarTextEngine::MoveSprite(int xC, int yC, std::string name) {
	Sprite curr = s.sprites.at(FindSpriteWithName(name));

	bool rL = false; // sprite - am i restricted from moving in a direction
	bool rR = false;
	bool rU = false;
	bool rD = false;

	for (int i = 0; i < curr.map.size(); i++) { // checks if any of his pixels are next to a wall and restricting movement
		if (curr.map.at(i).x == 0) {
			rL = true;
		}
		else if (curr.map.at(i).x == s.w - 1) {
			rR = true;
		}
		if (curr.map.at(i).y == 0) {
			rU = true;
		}
		else if (curr.map.at(i).y == s.h - 1) {
			rD = true;
		}
	}

	for (int i = 0; i < curr.map.size(); i++) {
		if (xC > 0 && !rR) {
			s.arr2D.at(curr.map.at(i).y).at(curr.map.at(i).x) = baseColor;
			curr.map.at(i).x += xC;
		}
		else if (xC < 0 && !rL) {
			s.arr2D.at(curr.map.at(i).y).at(curr.map.at(i).x) = baseColor;
			curr.map.at(i).x += xC;
		}
		if (yC < 0 && !rU) {
			s.arr2D.at(curr.map.at(i).y).at(curr.map.at(i).x) = baseColor;
			curr.map.at(i).y += yC;
		}
		else if (yC > 0 && !rD) {
			s.arr2D.at(curr.map.at(i).y).at(curr.map.at(i).x) = baseColor;
			curr.map.at(i).y += yC;
		}
	}
	s.sprites[FindSpriteWithName(name)] = curr;
}

void esb::EBollarTextEngine::Stop() {
	m_Running = false;
}

std::string esb::EBollarTextEngine::CheckCollide(std::string name) {
	Sprite curr = s.sprites.at(FindSpriteWithName(name));
	
	for (int i = 0; i < s.sprites.size(); i++) { // each sprite
		for (int q = 0; q < s.sprites.at(i).map.size(); q++) { // each vec2d in sprite
			if (i != FindSpriteWithName(name)) { // make sure not my sprite
				for (int w = 0; w < curr.map.size(); w++) { // each vec2d in my sprite
					if (curr.map.at(w).x == s.sprites.at(i).map.at(q).x && curr.map.at(w).y == s.sprites.at(i).map.at(q).y) {
						return s.sprites.at(i).name;
					}
				}
			}
		}
	}

	return "";
};


