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

	s.MakeSpriteRect(1, 1, 2, 2, esb::RED, "square");
	
	esb::Sprite square = s.FindSprite("square");
	e->SETREFRESHRATE(4);

	while (s.RUNNING) {
		if (e->ONKEY(esb::RIGHT)) {
			square.Translate(1, 0);
			s.DelSprite(s.FindSprite("square"));
			s.AddSprite(square);
		}
		if (e->ONKEY(esb::LEFT)) {
			square.Translate(-1, 0);
			s.DelSprite(s.FindSprite("square"));
			s.AddSprite(square);
		}
		if (e->ONKEY(esb::UP)) {
			square.Translate(0, -1);
			s.DelSprite(s.FindSprite("square"));
			s.AddSprite(square);
		}
		if (e->ONKEY(esb::DOWN)) {
			square.Translate(0, 1);
			s.DelSprite(s.FindSprite("square"));
			s.AddSprite(square);
		}


		e->RENDER(s);
	}

	system("PAUSE");
	return 0;
}