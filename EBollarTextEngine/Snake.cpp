#include "Snake.h"
#include <stdlib.h> // random

Snake::Snake() {
	game.ConstructScene(w, h, esb::BLACK);
	e->SETREFRESHRATE(10);

	head = game.MakeSpriteRect(headX, headY, 1, 1, esb::GREEN, "head");
	SpawnApple();
}

void Snake::Run() {
	while (e->RUNNING()) {
		MoveSnake();
		HandleEvents();

		e->RENDER(game);
	}
}

void Snake::MoveSnake() {
	apple->Translate(1, 0);
	if (snakeDir == "R") {
		head->Translate(1, 0);
	}
	else if (snakeDir == "L") {
		head->Translate(-1, 0);
	}
	else if (snakeDir == "U") {
		head->Translate(0, -1);
	}
	else if (snakeDir == "D") {
		head->Translate(0, 1);
	}

	for (int i = 1; i < snakeBody.size(); i++) {
		snakeBody.at(i)->setX(snakeBody.at(i - 1)->getX());
		snakeBody.at(i)->setY(snakeBody.at(i - 1)->getY());
	}
}

void Snake::HandleEvents() {
	GetInputs();
}

void Snake::SpawnApple() {
	int appleX = rand() % w;
	int appleY = rand() % h;

	apple = game.MakeSpriteRect(appleX, appleY, 1, 1, esb::RED, "apple");
}

void Snake::GetInputs() {
	if (snakeDir == "U" || snakeDir == "D") {
		if (e->ONKEY(esb::LEFTARROW)) {
			snakeDir = "L";
		} else if (e->ONKEY(esb::RIGHTARROW)) {
			snakeDir = "R";
		}
	}
	else {
		if (e->ONKEY(esb::UPARROW)) {
			snakeDir = "U";
		} else if (e->ONKEY(esb::DOWNARROW)) {
			snakeDir = "D";
		}
	}
}