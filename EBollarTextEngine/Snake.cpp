#include "Snake.h"
#include <iostream>

Snake::Snake() {
	eng.ConstructScene(w, h);
	eng.MakeSpriteRect(5, 5, 1, 1, esb::GREEN, "head");
	eng.SetBackground(esb::BLACK);
	eng.RefreshRate(30);
	point head;
	head.x = 5; head.y = 5;
	body.push_back(head);
	SpawnApple();
	eng.ShowConsoleCursor(false);
	GameLoop();
}

void Snake::GameLoop() {
	while (eng.gameLoop) {
		Movement();

		if (eng.CheckCollide("head") == "apple") {
			eng.DelSprite("apple");
			GrowBody();
			SpawnApple();
		}
		else if (eng.CheckCollide("head").substr(0, 4) == "body") {
			eng.Stop();
		}
		
		eng.Render();
	}
}

void Snake::Movement() {
	body.at(0).x = eng.getPosX("head") + 1; // update head pos in body vec
	body.at(0).y = eng.getPosY("head") + 1;

	if (body.size() > 1) {
		// update body points
		for (int i = body.size() - 1; i >= 1; i--) { // start at 1 because we skip over head
			if (wait > 0 && i == body.size() - 1) {
				wait--;
			}
			else {
				// draw each point
				eng.MoveSprite(body.at(i - 1).x - body.at(i).x, body.at(i - 1).y - body.at(i).y, "body" + std::to_string(i));
				body.at(i).x = body.at(i - 1).x;
				body.at(i).y = body.at(i - 1).y;
			}
		}
	}

	if (movingDir == esb::DOWN) {
		eng.MoveSprite(0, 1, "head");
	}
	if (movingDir == esb::UP) {
		eng.MoveSprite(0, -1, "head");
	}
	if (movingDir == esb::RIGHT) {
		eng.MoveSprite(2, 0, "head");
	}
	if (movingDir == esb::LEFT) {
		eng.MoveSprite(-2, 0, "head");
	}
	
	if (eng.OnKey(esb::UP) && movingDir != esb::DOWN) {
		movingDir = esb::UP;
	}
	if (eng.OnKey(esb::DOWN) && movingDir != esb::UP) {
		movingDir = esb::DOWN;
	}
	if (eng.OnKey(esb::RIGHT) && movingDir != esb::LEFT) {
		movingDir = esb::RIGHT;
	}
	if (eng.OnKey(esb::LEFT) && movingDir != esb::RIGHT) {
		movingDir = esb::LEFT;
	}
}

void Snake::GrowBody() {
	point newBody;
	newBody.x = body.at(body.size() - 1).x;
	newBody.y = body.at(body.size() - 1).y;

	eng.MakeSpriteRect(newBody.x, newBody.y, 1, 1, esb::GREEN, "body" + std::to_string(body.size()));
	wait = 1; // gotta wait for the other squares to move out the way

	body.push_back(newBody);
}

void Snake::SpawnApple() {
	eng.MakeSpriteRect(rand() % w + 1, rand() % h + 1, 1, 1, esb::RED, "apple");
}