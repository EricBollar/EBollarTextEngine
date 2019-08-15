/*
#include "Menu.h"

void Menu::LoadStart(){
	m.ConstructScene(50, 25);
	m.SetBackground(esb::BLACK);
	m.RefreshRate(1);
	
	MakeTitle();
	m.MakeText(1, 1, esb::BLACK, "Eric Bollar's", "ericbollar");
	m.MakeSpriteRect(1, 17, 9, 50, esb::WHITE, "background");
	m.MakeSpriteRect(46, 18, 1, 1, esb::BLUE, "cursor");
	m.MakeText(50, 18, esb::LIGHTBLUE, "Play Snake", "playsnake");
	m.MakeText(50, 20, esb::LIGHTBLUE, "Play Pong With Someone", "playpongpvp");
	m.MakeText(50, 22, esb::LIGHTBLUE, "Settings", "settings");
	m.MakeText(50, 24, esb::LIGHTBLUE, "Credits", "credits");
	m.MakeText(21, 18, esb::GREEN, "CONTROLS", "menucontrols");
	m.MakeText(9, 20, esb::WHITE, "USE UP & DOWN ARROWS TO NAVIGATE", "updowncontrols");
	m.MakeText(12, 22, esb::WHITE, "PRESS RIGHT ARROW TO SELECT", "rightcontrols");

	m.ShowConsoleCursor(false);
	MenuLoop();
}

void Menu::MenuLoop() {
	while (m.gameLoop) {
		MoveCursor();

		m.Render();
		if (buttonWait > 0) {
			buttonWait--;
		}
	};
}

void Menu::LoadGame() {

}

void Menu::MoveCursor() {
	if (m.OnKey(esb::DOWN) && cursorPos < 3 && buttonWait <= 0) {
		m.MoveSprite(0, 1, "cursor");
		m.MoveSprite(0, 1, "cursor");
		cursorPos++;
		buttonWait = 1;
	}
	if (m.OnKey(esb::UP) && cursorPos > 0 && buttonWait <= 0) {
		m.MoveSprite(0, -1, "cursor");
		m.MoveSprite(0, -1, "cursor");
		cursorPos--;
		buttonWait = 1;
	}
	if (m.OnKey(esb::RIGHT)) {

	}
}

//pretty title
void Menu::MakeTitle() {
	// make some pretty tittle here eventually
}
*/