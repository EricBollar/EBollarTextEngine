#include "Snake.h"
#include <stdlib.h> // random

Snake::Snake() {
	game.ConstructScene(w, h, esb::BLACK);
	e->SETREFRESHRATE(10);

	head = game.MakeSpriteRect(headX, headY, 1, 1, esb::GREEN, "head");
	head->setY(2);
	snakeBody.push_back(head);
	SpawnApple();

	esb::Sprite* startingBody = game.MakeSpriteRect(headX, headY, 1, 1, esb::GREEN, "body");
	snakeBody.push_back(startingBody);
}

void Snake::Run() {
	while (e->RUNNING()) {
		MoveSnake();
		HandleEvents();

		e->RENDER(game);
	}
}

void Snake::MoveSnake() {
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
		snakeBody.at(i)->setX(snakeBody.at(i - 1)->getX() + 3);
		snakeBody.at(i)->setY(snakeBody.at(i - 1)->getY() - 1);
	}
}

void Snake::HandleEvents() {
	GetInputs();
	if (game.CheckSpriteCollide(*head) == "apple") {
		SpawnBody();
		game.DelSprite(*apple);
		SpawnApple();
	}
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

void Snake::SpawnBody() {
	int bodyX = snakeBody.at(snakeBody.size() - 1)->getX();
	int bodyY = snakeBody.at(snakeBody.size() - 1)->getY();

	esb::Sprite* body = game.MakeSpriteRect(bodyX, bodyY, 1, 1, esb::GREEN, "body");
	snakeBody.push_back(body);
}