#pragma once
#include "EBollarTextEngine.h"

class PongPVP {
	enum ballDir {
		UPRIGHT,
		UPLEFT,
		DOWNRIGHT,
		DOWNLEFT
	};
	enum paddleDir {
		UP,
		DOWN
	};
private:
	int ballSpeed = 1;
	int hits = 1;
	bool ballgo = false;
	int leftScore = 0;
	int rightScore = 0;
	ballDir ballVel = UPRIGHT;
	paddleDir rPaddleDir = UP;
	paddleDir lPaddleDir = UP;
	int lPaddleX;
	int lPaddleY;
	int rPaddleX;
	int rPaddleY;
	int ballX;
	int ballY;
	int w = 50;
	int h = 20;
	esb::EBollarTextEngine eng;
public:
	PongPVP();
	void GameLoop();
	void movePaddles();
	void moveBall();
};