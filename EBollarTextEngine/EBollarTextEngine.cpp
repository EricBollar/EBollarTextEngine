#include "EBollarTextEngine.h"
#include <iostream>
#include <io.h>
#include <fcntl.h>
#include <string>
#include <vector>
#include <stdlib.h>
#include <thread>
#include <chrono>
#include <Windows.h>

void esb::EBollarTextEngine::cls() {
	static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

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

void esb::EBollarTextEngine::setCursorPosition(int x, int y)
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

void esb::EBollarTextEngine::setConsoleColour(unsigned short colour)
{
	static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	std::cout.flush();
	SetConsoleTextAttribute(hOut, colour);
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

void esb::EBollarTextEngine::FillScene(wchar_t shade) {
	s.arr2D.clear();
	s.x.clear();
	for (int q = 0; q < s.w; q++) {
		for (int i = 0; i < s.h; i++) {
			s.x.push_back(shade);
		}
		s.arr2D.push_back(s.x);
		s.x.clear();
	}
}

bool esb::EBollarTextEngine::OnKey(keyCode key) {
	switch (key) {
		case RIGHT:
			if (GetKeyState(VK_RIGHT) & 0x8000 && canMove)
			{
				return true;
				canMove = false;
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

wchar_t esb::EBollarTextEngine::sR(int i) {
	switch (i) {
		case 1:
			return L'\u2593';
		case 2:
			return L'\u2592';
		case 3:
			return L'\u2591';
		case 4:
			return L'\u2588';
	}
	std::cout << "error";
	return L' ';
}

std::vector<vec2D> esb::EBollarTextEngine::CompareFrames(std::vector<std::vector<wchar_t> >& frame1, std::vector<std::vector<wchar_t> >& frame2) {
	std::vector<vec2D> diffs;

	for (int q = 0; q < frame1.size(); q++) {
		for (int i = 0; i < frame1.at(q).size(); i++) {
			if (frame1.at(q).at(i) != frame2.at(q).at(i)) {
				vec2D curr;
				curr.x = i;
				curr.y = q;
				curr.shade = frame2.at(q).at(i);
				diffs.push_back(curr);
			}
		}
	}

	return diffs;
};

void esb::EBollarTextEngine::PrintScene() {
	FillScene(baseFill);
	SpriteCollate();

	setConsoleColour(0x1a);	

	std::vector<vec2D> diffs = CompareFrames(s.prevFrame, s.arr2D);

	for (int i = 0; i < diffs.size(); i++) {
		setCursorPosition(diffs.at(i).x, diffs.at(i).y);
		std::wcout << diffs.at(i).shade;
	}

	std::cout.flush();
	s.prevFrame = s.arr2D;
}

void esb::EBollarTextEngine::PaintPix(int x, int y, wchar_t shade) {
	x--; y--;
	s.arr2D.at(y).at(x) = shade;
}

void esb::EBollarTextEngine::MakeSpriteRect(int x, int y, int w, int h, wchar_t shade, std::string name) {
	Sprite a;
	x--; y--;
	a.name = name;

	for (int q = 0; q < h; q++) { // sets width and height
		for (int i = 0; i < w; i++) {
			vec2D curr;
			curr.x = x + i;
			curr.y = y + q;
			curr.shade = shade;
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

void esb::EBollarTextEngine::SpriteCollate() {
	if (s.sprites.size() > 0) {
		for (int sprts = 0; sprts < s.sprites.size(); sprts++) { // current sprite
			for (int i = 0; i < s.sprites.at(sprts).map.size(); i++) { // current vec2D
				Sprite spr = s.sprites.at(sprts);
				s.arr2D.at(spr.map[i].y).at(spr.map[i].x) = spr.map[i].shade;
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

void esb::EBollarTextEngine::ConstructScene(int height, int width) {
	s.w = width;
	s.h = height;
	
	for (int q = 0; q < s.w; q++) {
		for (int i = 0; i < s.h; i++) {
			s.filler.push_back(sR(4));
		}
		s.prevFrame.push_back(s.filler);
		s.filler.clear();
	}

	esb::EBollarTextEngine::ClearSprites();
	s.arr2D.clear();
	s.x.clear();
	for (int q = 0; q < s.w; q++) {
		for (int i = 0; i < s.h; i++) {
			s.x.push_back(L' ');
		}
		s.arr2D.push_back(s.x);
		s.x.clear();
	}

	m_Running = true;
	gameLoop = m_Running;
}


void esb::EBollarTextEngine::MoveSprite(int xC, int yC, std::string name) {
	Sprite curr = s.sprites.at(FindSpriteWithName(name));

	bool rL = false;
	bool rR = false;
	bool rU = false;
	bool rD = false;

	for (int i = 0; i < curr.map.size(); i++) {
		if (curr.map.at(i).x == 0) {
			rL = true;
		}
		else if (curr.map.at(i).x == s.h - 1) {
			rR = true;
		}
		if (curr.map.at(i).y == 0) {
			rU = true;
		}
		else if (curr.map.at(i).y == s.w - 1) {
			rD = true;
		}
	}

	for (int i = 0; i < curr.map.size(); i++) {
		if (xC > 0 && !rR) {
			s.arr2D.at(curr.map.at(i).y).at(curr.map.at(i).x) = L' ';
			curr.map.at(i).x += xC;
		}
		else if (xC < 0 && !rL) {
			s.arr2D.at(curr.map.at(i).y).at(curr.map.at(i).x) = L' ';
			curr.map.at(i).x += xC;
		}
		if (yC < 0 && !rU) {
			s.arr2D.at(curr.map.at(i).y).at(curr.map.at(i).x) = L' ';
			curr.map.at(i).y += yC;
		}
		else if (yC > 0 && !rD) {
			s.arr2D.at(curr.map.at(i).y).at(curr.map.at(i).x) = L' ';
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


