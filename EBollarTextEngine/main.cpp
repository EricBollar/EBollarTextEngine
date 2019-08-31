#include <iostream>
#include <windows.h>
#include <stdio.h>
#include "EBollarTextEngine.h"

int main()
{
	esb::Engine* e = esb::Engine::Instance();
	esb::Scene s;
	
	s.ConstructScene(20, 15);
	s.SetBackground(esb::LIGHTBLUE);

	s.MakeSpriteRect(8, 4, 1, 1, esb::RED, "square");
	
	esb::Sprite square = s.FindSprite("square");
	e->SETREFRESHRATE(4);

	while (s.RUNNING) {
		if (e->ONKEY(esb::RIGHT)) {
			s.moveSprite(&square, 1, 0);
		}
		if (e->ONKEY(esb::LEFT)) {
			s.moveSprite(&square, -1, 0);
		}
		if (e->ONKEY(esb::UP)) {
			s.moveSprite(&square, 0, -1);
		}
		if (e->ONKEY(esb::DOWN)) {
			s.moveSprite(&square, 0, 1);
		}

		e->RENDER(s);
	}

	system("PAUSE");
	return 0;
}