#include "EBollarTextEngine.h"
#include <stddef.h>
#include <iostream>

esb::Engine* instance = NULL; // global static pointer to check for other existing engines

esb::Engine* esb::Engine::Instance() {
	if (!instance) {
		instance = new Engine;
	}

	return instance;
}

void esb::Engine::LOADSCENE(Scene s) {
	currScene = s;
}

void esb::Engine::SETREFRESHRATE(int milliseconds) {
	rRate = milliseconds;
}

bool esb::Engine::ONKEY(esb::direction key) { // this function just returns if designated key is pressed or not
	switch (key) {
	case esb::RIGHT:
		if (GetKeyState(VK_RIGHT) & 0x8000)
		{
			return true;
		}
		else {
			return false;
		}
	case esb::LEFT:
		if (GetKeyState(VK_LEFT) & 0x8000)
		{
			return true;
		}
		else {
			return false;
		}
	case esb::UP:
		if (GetKeyState(VK_UP) & 0x8000)
		{
			return true;
		}
		else {
			return false;
		}
	case esb::DOWN:
		if (GetKeyState(VK_DOWN) & 0x8000)
		{
			return true;
		}
		else {
			return false;
		}
	}
}

void cls() {
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

void setCursorPosition(int x, int y) // puts cursor at point on screen - allows for double buffering
{
	static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	std::cout.flush();
	COORD coord = { (SHORT)x, (SHORT)y };
	SetConsoleCursorPosition(hOut, coord);
}

void ShowConsoleCursor(bool ysno)
{
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_CURSOR_INFO     cursorInfo;

	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = ysno; // sets the cursor visibility
	SetConsoleCursorInfo(out, &cursorInfo);
}

void setConsoleColour(WORD* Attributes, DWORD Color) // sets color of next print
{
	CONSOLE_SCREEN_BUFFER_INFO Info;
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(hStdout, &Info);
	*Attributes = Info.wAttributes;
	SetConsoleTextAttribute(hStdout, Color);
}

void ResetConsoleColour(WORD Attributes) // sets color to blank
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Attributes);
}

DWORD esb::Engine::GETCOLOR(esb::color c) { // makes things easier i think
	switch (c) {
	case esb::BLUE:
		return BACKGROUND_BLUE;
	case esb::RED:
		return BACKGROUND_RED;
	case esb::GREEN:
		return BACKGROUND_GREEN;
	case esb::YELLOW:
		return (BACKGROUND_RED | BACKGROUND_GREEN);
	case esb::PINK:
		return (BACKGROUND_RED | BACKGROUND_BLUE);
	case esb::LIGHTBLUE:
		return (BACKGROUND_GREEN | BACKGROUND_BLUE);
	case esb::BLACK:
		return (BACKGROUND_INTENSITY);
	case esb::WHITE:
		return (BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
	}
}

void esb::Engine::PrintScene() {
	ShowConsoleCursor(false);
	std::vector<std::vector<Pixel> > currFrame = currScene.getFrame();
	std::vector<Pixel> diffs = currScene.getDiffs();

	// FillScene(baseColor); // easy background and reset (although not most efficient this is windows console so i don't think its a huge problem...)
	// SpriteCollate(); // put them on de top uh de ting
	setCursorPosition(0, 0);

	// std::vector<Pixel> diffs = CompareFrames(prevFrame, arr2D);

	for (int i = 0; i < diffs.size(); i++) {
		setCursorPosition(diffs.at(i).x * 2, diffs.at(i).y);

		setConsoleColour(&Attributes, BACKGROUND_INTENSITY | GETCOLOR((diffs.at(i).c))); // this just sets the color of the current pixel that it's printing to the console's color scheme
		std::cout << diffs.at(i).t << diffs.at(i).t; // woo hoo it prints a thing (print twice to adjust for vertical rectangular character borders so [][] instead of [] - makes it look more like a square

		ResetConsoleColour(Attributes); // this is only useful once we've finished printing a frame so the things don't cross over to next frame
	}

	if (currScene.RUNNING == false) { // if game loop ends put cursor just below entire frame so it gets out the way or if u wanna print additional messages
		setCursorPosition(0, currScene.getH());
	}

	std::cout.flush();
	prevFrame = currScene.getFrame(); // now do it all again bub
}

void esb::Engine::RENDER() {
	currScene.Process();
	PrintScene();
	std::this_thread::sleep_for(std::chrono::milliseconds(rRate)); // adjustable delay
}