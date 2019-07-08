#include "PongPVP.h"
#include <iostream>

PongPVP::PongPVP() {
	eng.ConstructScene(w, h);
	eng.MakeText(8, 15, esb::GREEN, std::to_string(leftScore), "lScore");
	eng.MakeText(14, 4, esb::GREEN, "hello world!", "rScore");
	eng.MakeSpriteRect(7, 1, 1, w - 6, esb::BLACK, "top");
	eng.MakeSpriteRect(7, h, 1, w - 6, esb::BLACK, "bot");
	eng.MakeSpriteRect(5, h/2, h/4, 1, esb::RED, "lPaddle");
	eng.MakeSpriteRect(w*2 - 5, h/2, h/4, 1, esb::RED, "rPaddle");
	eng.MakeSpriteRect(8, h/2, 1, 1, esb::BLUE, "ball");
	eng.SetBackground(esb::BLACK);
	eng.RefreshRate(15);
	
	eng.ShowConsoleCursor(false);
	GameLoop();
}

void PongPVP::GameLoop() {
	while (eng.gameLoop) {
		ballgo = true;
		eng.setText(std::to_string(leftScore), "lScore");
		leftScore++;
		movePaddles();
		moveBall();
		eng.Render();
	}
}

void PongPVP::movePaddles() {
	if (eng.OnKey(esb::UP)) {
		eng.MoveSprite(0, -1, "rPaddle");
		rPaddleDir = UP;
	}
	if (eng.OnKey(esb::DOWN)) {
		eng.MoveSprite(0, 1, "rPaddle");
		rPaddleDir = DOWN;
	}
	if (eng.OnKey(esb::W)) {
		eng.MoveSprite(0, -1, "lPaddle");
		lPaddleDir = UP;
	}
	if (eng.OnKey(esb::S)) {
		eng.MoveSprite(0, 1, "lPaddle");
		lPaddleDir = DOWN;
	}
}

void PongPVP::moveBall() {

	if (ballVel == UPRIGHT) {
		eng.MoveSprite(ballSpeed, -ballSpeed, "ball");
	}
	if (ballVel == UPLEFT) {
		eng.MoveSprite(-ballSpeed, -ballSpeed, "ball");
	}
	if (ballVel == DOWNRIGHT) {
		eng.MoveSprite(ballSpeed, ballSpeed, "ball");
	}
	if (ballVel == DOWNLEFT) {
		eng.MoveSprite(-ballSpeed, ballSpeed, "ball");
	}

	if (eng.CheckCollide("ball") == "top" && ballVel == UPRIGHT) {
		ballVel = DOWNRIGHT;
	} else if (eng.CheckCollide("ball") == "top" && ballVel == UPLEFT) {
		ballVel = DOWNLEFT;
	}
	if (eng.CheckCollide("ball") == "bot" && ballVel == DOWNRIGHT) {
		ballVel = UPRIGHT;
	}
	else if (eng.CheckCollide("ball") == "bot" && ballVel == DOWNLEFT) {
		ballVel = UPLEFT;
	}
	if (eng.CheckCollide("ball") == "rPaddle" && rPaddleDir == UP) {
		ballVel = UPLEFT;
	}
	else if (eng.CheckCollide("ball") == "rPaddle" && rPaddleDir == DOWN) {
		ballVel = DOWNLEFT;
	}
	if (eng.CheckCollide("ball") == "lPaddle" && lPaddleDir == UP) {
		ballVel = UPRIGHT;
	}
	else if (eng.CheckCollide("ball") == "lPaddle" && lPaddleDir == DOWN) {
		ballVel = DOWNRIGHT;
	}

	if (hits % 8 == 0) {
		ballSpeed++;
	}
}