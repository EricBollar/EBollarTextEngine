#include "Snake.h"
#include <stdlib.h> // random

Snake::Snake() {
	e->SETREFRESHRATE(85);
	SetupGame();
	Run();
}

void Snake::Run() {
	while (e->RUNNING()) {
		if (e->ONKEY(esb::ESCAPE)) {
			e->STOP();
		}
		if (e->GETSCENE() == &gameS) {
			MoveSnake();
			HandleEvents();
		}
		else if (e->GETSCENE() == &menuS) {
			if (e->ONKEY(esb::SPACE)) {
				e->LOAD(&gameS);
			}
		}
		else if (e->GETSCENE() == &deathS && e->ONKEY(esb::ENTER)) {
			SetupGame();
			e->LOAD(&menuS);
		}

		e->RENDER();
	}
}

void Snake::SetupGame() {
	resetVars();
	gameS.ConstructScene(w, h, esb::BLACK);
	deathS.ConstructScene(w, h, esb::RED);
	menuS.ConstructScene(w, h, esb::BLACK);

	e->LOAD(&menuS);

	createSprites();
	SpawnApple();
}

void Snake::resetVars() {
	snakeBody.clear();
	cursorPos = 0;

	headX = w / 2;
	headY = h / 2;
	wait = false;

	snakeDir = "R";
	prevDir = "R";
}

void Snake::createSprites() {
	returnToMenuText = deathS.MakeText(6, h / 2, esb::BLACK, "PRESS ENTER FOR MENU", "deathText");
	playGameText = menuS.MakeText(6, 12, esb::WHITE, "PRESS SPACE TO PLAY", "playGame");
	title = menuS.MakeText(w/2 - 3, 6, esb::GREEN, "WELCOME...", "w");
	titleT = menuS.MakeText(w/2 + 1, 7, esb::GREEN, "TO SNAKE", "s");

	head = gameS.MakeSpriteRect(headX, headY, 1, 1, esb::GREEN, "head");
	snakeBody.push_back(head);

	esb::Sprite* startingBody = gameS.MakeSpriteRect(headX - 1, headY, 1, 1, esb::GREEN, "body");
	snakeBody.push_back(startingBody);
}

void Snake::MoveSnake() {
	for (int i = snakeBody.size() - 1; i > 0; i--) {
		if (!wait && i == snakeBody.size() - 1) {
			snakeBody.at(i)->setX(snakeBody.at(i - 1)->getX());
			snakeBody.at(i)->setY(snakeBody.at(i - 1)->getY());
		}
		else if (wait == true && i == snakeBody.size() - 1) {
			wait = false;
		}
		else {
			snakeBody.at(i)->setX(snakeBody.at(i - 1)->getX());
			snakeBody.at(i)->setY(snakeBody.at(i - 1)->getY());
		}
	}

	prevDir = snakeDir;
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
	if (gameS.CheckSpriteCollide(head, apple)) {
		SpawnBody();
		gameS.DelSprite(*apple);
		SpawnApple();
	}
	for (int i = 1; i < snakeBody.size() - 1; i++) {
		if (gameS.CheckSpriteCollide(head, snakeBody.at(i))) {
			e->LOAD(&deathS);
			break;
		}
	}
}

void Snake::SpawnApple() {
	int appleX = rand() % w + 1;
	int appleY = rand() % h + 1;

	apple = gameS.MakeSpriteRect(appleX, appleY, 1, 1, esb::RED, "apple");
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

	esb::Sprite* body = gameS.MakeSpriteRect(bodyX, bodyY, 1, 1, esb::GREEN, "body");
	snakeBody.push_back(body);
	wait = true;
}