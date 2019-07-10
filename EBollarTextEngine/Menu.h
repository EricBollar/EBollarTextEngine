#pragma once
#include "EBollarTextEngine.h"

class Menu {
private:
	esb::EBollarTextEngine m;
	int cursorPos = 0;
	int buttonWait = 1;

	std::vector<esb::vec2D> menuscreenmap;
public:
	void MakeTitle();
	void LoadStart();
	void LoadGame();
	void MoveCursor();
	void MenuLoop();
};