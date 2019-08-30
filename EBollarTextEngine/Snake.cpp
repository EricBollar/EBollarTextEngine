#include "Snake.h"
#include <iostream>

Snake::Snake() {
	s.ConstructScene(w, h);
	s.MakeSpriteRect(5, 5, 1, 1, esb::GREEN, "head", 1);
	s.SetBackground(esb::BLACK);
	e->SETREFRESHRATE(40);
	point head;
	head.x = 5; head.y = 5;
	body.push_back(head);
	SpawnApple();
	Run();
}

void Snake::Run() {
	while (s.RUNNING) {
		Movement();

		if (s.CheckSpriteCollide(s.FindSprite("head")).getName() == "apple") {
			s.DelSprite(s.FindSprite("apple"));
			GrowBody();
			SpawnApple();
		}
		else if (s.CheckSpriteCollide(s.FindSprite("head")).getName().substr(0, 4) == "body") {
			s.Stop();
		}

		e->RENDER(s);
	}
}

void Snake::Movement() {
	body.at(0).x = s.FindSprite("head").getX() + 1; // update head pos in body vec
	body.at(0).y = s.FindSprite("head").getY() + 1;

	if (body.size() > 1) {
		// update body points
		for (int i = body.size() - 1; i >= 1; i--) { // start at 1 because we skip over head
			if (wait > 0 && i == body.size() - 1) {
				wait--;
			}
			else {
				// draw each point
				s.FindSprite("body" + std::to_string(i)).Translate(body.at(i - 1).x - body.at(i).x, body.at(i - 1).y - body.at(i).y);
				body.at(i).x = body.at(i - 1).x;
				body.at(i).y = body.at(i - 1).y;
			}
		}
	}

	if (movingDir == esb::DOWN) {
		s.FindSprite("head").Translate(0, 2);
	}
	if (movingDir == esb::UP) {
		s.FindSprite("head").Translate(0, -2);
	}
	if (movingDir == esb::RIGHT) {
		s.FindSprite("head").Translate(2, 0);
	}
	if (movingDir == esb::LEFT) {
		s.FindSprite("head").Translate(-2, 0);
	}
	
	if (e->ONKEY(esb::UP) && movingDir != esb::DOWN) {
		movingDir = esb::UP;
	}
	if (e->ONKEY(esb::DOWN) && movingDir != esb::UP) {
		movingDir = esb::DOWN;
	}
	if (e->ONKEY(esb::RIGHT) && movingDir != esb::LEFT) {
		movingDir = esb::RIGHT;
	}
	if (e->ONKEY(esb::LEFT) && movingDir != esb::RIGHT) {
		movingDir = esb::LEFT;
	}
}

void Snake::GrowBody() {
	point newBody;
	newBody.x = body.at(body.size() - 1).x;
	newBody.y = body.at(body.size() - 1).y;

	s.MakeSpriteRect(newBody.x, newBody.y, 1, 1, esb::GREEN, "body" + std::to_string(body.size()));
	wait = 2; // gotta wait for the other squares to move out the way

	body.push_back(newBody);
}

void Snake::SpawnApple() {
	s.MakeSpriteRect(rand() % w + 1, rand() % h + 1, 1, 1, esb::RED, "apple");
}