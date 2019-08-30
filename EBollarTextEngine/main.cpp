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

	e->LOADSCENE(s);
	e->SETREFRESHRATE(25);

	while (s.RUNNING) {

		if (e->ONKEY(esb::RIGHT)) {
			square.Translate(1, 0);
		}

		e->RENDER();
	}

	system("PAUSE");
	return 0;
}