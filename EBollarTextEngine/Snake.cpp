#include "Snake.h"
#include <stdlib.h> // random

Snake::Snake() {
	game.ConstructScene(w, h, esb::BLACK);
	death.ConstructScene(w, h, esb::RED);
	menu.ConstructScene(w, h, esb::BLACK);
	e->SETREFRESHRATE(5);

	
	cursor = menu.MakeSpriteRect(5, 12, 1, 2, esb::WHITE, "cursor");
	returnToMenu = death.MakeText(w / 2 - 6, h / 2, esb::BLACK, "PRESS ENTER TO RETURN TO MENU", "deathText");
	

	head = game.MakeSpriteRect(headX, headY, 1, 1, esb::GREEN, "head");
	head->setY(2);
	snakeBody.push_back(head);
	SpawnApple();

	esb::Sprite* startingBody = game.MakeSpriteRect(headX - 1, headY, 1, 1, esb::GREEN, "body");
	snakeBody.push_back(startingBody);
}

void Snake::Run() {
	while (e->RUNNING()) {
		if (e->ONKEY(esb::ESCAPE)) {
			e->STOP();
		}
		if (currScene == &game) {
			MoveSnake();
			HandleEvents();
		}
		else if (currScene == &menu) {
			MoveCursor();
		}
		else if (currScene == &death) {
			if (e->ONKEY(esb::ENTER)) {
				currScene = &menu;
			}
		}

		e->RENDER(*currScene);
	}
}

void Snake::MoveCursor() {
	if (e->ONKEY(esb::DOWNARROW) && cursorPos == 0) {
		cursor->Translate(0, 5);
		cursorPos = 1;
	} 
	else if (e->ONKEY(esb::UPARROW) && cursorPos == 1) {
		cursor->Translate(0, -5);
		cursorPos = 0;
	}
}

void Snake::MoveSnake() {
	for (int i = snakeBody.size() - 1; i > 0; i--) {
		snakeBody.at(i)->setX(snakeBody.at(i - 1)->getX());
		snakeBody.at(i)->setY(snakeBody.at(i - 1)->getY());
	}

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
}

void Snake::HandleEvents() {
	GetInputs();
	if (game.CheckSpriteCollide(head, apple)) {
		SpawnBody();
		game.DelSprite(*apple);
		SpawnApple();
	}
	for (int i = 1; i < snakeBody.size() - 1; i++) {
		if (game.CheckSpriteCollide(head, snakeBody.at(i))) {
			currScene = &death;
		}
	}
}

void Snake::SpawnApple() {
	int appleX = rand() % w + 1;
	int appleY = rand() % h + 1;

	apple = game.MakeSpriteRect(appleX, appleY, 1, 1, esb::RED, "apple");
}

void Snake::GetInputs() {
	if (snakeDir == "U" || snakeDir == "D") {
		if (e->ONKEY(esb::LEFTARROW)) {
			prevDir = snakeDir;
			snakeDir = "L";
		} else if (e->ONKEY(esb::RIGHTARROW)) {
			prevDir = snakeDir;
			snakeDir = "R";
		}
	}
	else {
		if (e->ONKEY(esb::UPARROW)) {
			prevDir = snakeDir;
			snakeDir = "U";
		} else if (e->ONKEY(esb::DOWNARROW)) {
			prevDir = snakeDir;
			snakeDir = "D";
		}
	}
}

void Snake::SpawnBody() {
	int bodyX = snakeBody.at(snakeBody.size() - 1)->getX() + 1;
	int bodyY = snakeBody.at(snakeBody.size() - 1)->getY() + 1;

	
	if (snakeDir == "R") {
		bodyX--;
	} 
	else if (snakeDir == "L") {
		bodyX++;
	} 
	else if (snakeDir == "U") {
		bodyY++;
	}
	else {
		bodyY--;
	}

	esb::Sprite* body = game.MakeSpriteRect(bodyX, bodyY, 1, 1, esb::GREEN, "body");
	snakeBody.push_back(body);
}