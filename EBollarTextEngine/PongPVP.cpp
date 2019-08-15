/*
#include "PongPVP.h"
#include <iostream>

PongPVP::PongPVP() {
	eng.ConstructScene(w, h);
	eng.MakeText(w - 4, h/2 - 5, esb::BLACK, std::to_string(leftScore), "lScore"); 
	eng.MakeText(w + 4, h/2 - 5, esb::BLACK, "hello world!", "rScore");
	eng.MakeText(w - 2, h / 2 - 6, esb::BLACK, "Score", "scoreboardText");
	eng.MakeText(w - 7, h/2 - 4, esb::BLACK, "First to 5 wins!", "encouragemetText");
	eng.MakeText(w, h / 2 - 5, esb::BLACK, "-", "scoreboardHash");
	eng.MakeSpriteRect(7, 1, 1, w - 6, esb::BLACK, "top");
	eng.MakeSpriteRect(7, h, 1, w - 6, esb::BLACK, "bot");
	eng.MakeSpriteRect(5, h/2, h/4, 1, esb::RED, "lPaddle");
	eng.MakeSpriteRect(w*2 - 5, h/2, h/4, 1, esb::RED, "rPaddle");
	eng.MakeSpriteRect(18, h/2, 1, 1, esb::BLUE, "ball");
	eng.SetBackground(esb::BLACK);
	eng.RefreshRate(refreshRate);
	
	eng.ShowConsoleCursor(false);
	GameLoop();
}

void PongPVP::GameLoop() {
	while (eng.gameLoop) {
		ballgo = true;
		movePaddles();
		moveBall();
		checkPoints();
		eng.Render();
	}
}

void PongPVP::checkPoints() {
	bool reset = false;
	if (eng.getPosX("ball") <= 0) {
		rightScore++;
		reset = true;
		ballVel = LEFT;
	}
	else if (eng.getPosX("ball") >= w * 2 - 2) {
		leftScore++;
		reset = true;
		ballVel = RIGHT;
	}

	if (leftScore >= 5) {
		eng.DelSprite("encouragemetText");
		eng.MakeText(w - 7, h / 2 - 4, esb::BLACK, "Left Player Wins!", "winner");
		eng.Stop();
	}
	else if (rightScore >= 5) {
		eng.DelSprite("encouragemetText");
		eng.MakeText(w - 7, h / 2 - 4, esb::BLACK, "Right Player Wins!", "winner");
		eng.Stop();
	}else if (reset) {
		rPaddleDir = NONE;
		lPaddleDir = NONE;
		eng.setX(w / 2, "ball");
		eng.setY(h / 2, "ball");
		hits = 0;
		ballSpeed = 10;
	}
	eng.setText(std::to_string(rightScore), "rScore");
	eng.setText(std::to_string(leftScore), "lScore");
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
		eng.MoveSprite(1, -1, "ball");
		if (ballBoost == 0) {
			eng.MoveSprite(1, -1, "ball");
			ballBoost = ballSpeed;
		}
	}else if (ballVel == UPLEFT) {
		eng.MoveSprite(-1, -1, "ball");
		if (ballBoost == 0) {
			eng.MoveSprite(-1, -1, "ball");
			ballBoost = ballSpeed;
		}
	}else if (ballVel == DOWNRIGHT) {
		eng.MoveSprite(1, 1, "ball");
		if (ballBoost == 0) {
			eng.MoveSprite(1, 1, "ball");
			ballBoost = ballSpeed;
		}
	}else if (ballVel == DOWNLEFT) {
		eng.MoveSprite(-1, 1, "ball");
		if (ballBoost == 0) {
			eng.MoveSprite(-1, 1, "ball");
			ballBoost = ballSpeed;
		}
	}else if (ballVel == LEFT) {
		eng.MoveSprite(-1, 0, "ball");
		if (ballBoost == 0) {
			eng.MoveSprite(-1, 0, "ball");
			ballBoost = ballSpeed;
		}
	}else if (ballVel == RIGHT) {
		eng.MoveSprite(1, 0, "ball");
		if (ballBoost == 0) {
			eng.MoveSprite(1, 0, "ball");
			ballBoost = ballSpeed;
		}
	}

	if (eng.CheckCollide("ball") == "top" && ballVel == UPRIGHT) {
		ballVel = DOWNRIGHT;
	} else if (eng.CheckCollide("ball") == "top" && ballVel == UPLEFT) {
		ballVel = DOWNLEFT;
	}else if (eng.CheckCollide("ball") == "bot" && ballVel == DOWNRIGHT) {
		ballVel = UPRIGHT;
	}
	else if (eng.CheckCollide("ball") == "bot" && ballVel == DOWNLEFT) {
		ballVel = UPLEFT;
	}else if (eng.CheckCollide("ball") == "rPaddle" && rPaddleDir == UP) {
		ballVel = UPLEFT;
		hits++;
	}
	else if (eng.CheckCollide("ball") == "rPaddle" && rPaddleDir == DOWN) {
		ballVel = DOWNLEFT;
		hits++;
	}else if (eng.CheckCollide("ball") == "lPaddle" && lPaddleDir == UP) {
		ballVel = UPRIGHT;
		hits++;
	}
	else if (eng.CheckCollide("ball") == "lPaddle" && lPaddleDir == DOWN) {
		ballVel = DOWNRIGHT;
		hits++;
	}
	else if (eng.CheckCollide("ball") == "lPaddle" && lPaddleDir == NONE) {
		ballVel = RIGHT;
		hits++;
	}
	else if (eng.CheckCollide("ball") == "rPaddle" && lPaddleDir == NONE) {
		ballVel = LEFT;
		hits++;
	}

	if (hits % 3 == 0 && ballSpeed > 1) {
		ballSpeed--;
		hits = 3;
	}
	ballBoost--;
}
*/