#pragma once
#include "EBollarTextEngine.h"

class PongPVP {
	enum ballDir {
		UPRIGHT,
		UPLEFT,
		DOWNRIGHT,
		DOWNLEFT,
		RIGHT,
		LEFT
	};
	enum paddleDir {
		UP,
		DOWN,
		NONE
	};
private:
	int ballSpeed = 10;
	int hits = 0;
	bool ballgo = false;
	int ballBoost = ballSpeed;
	int leftScore = 0;
	int rightScore = 0;
	int refreshRate = 15;
	ballDir ballVel = RIGHT;
	paddleDir rPaddleDir = NONE;
	paddleDir lPaddleDir = NONE;
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
	void checkPoints();
	void GameLoop();
	void movePaddles();
	void moveBall();
};