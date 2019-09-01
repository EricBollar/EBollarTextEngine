#include <iostream>
#include <windows.h>
#include <stdio.h>
#include "Snake.h"

void Demo() {
	esb::Engine* e = esb::Engine::Instance();
	esb::Scene s;

	s.ConstructScene(20, 15, esb::LIGHTBLUE);

	esb::Sprite* square = s.MakeSpriteRect(8, 4, 1, 1, esb::RED, "square");
	e->SETREFRESHRATE(4);

	while (e->RUNNING()) {
		if (e->ONKEY(esb::RIGHTARROW)) {
			square->Translate(1, 0);
		}
		if (e->ONKEY(esb::LEFTARROW)) {
			square->Translate(-1, 0);
		}
		if (e->ONKEY(esb::UPARROW)) {
			square->Translate(0, -1);
		}
		if (e->ONKEY(esb::DOWNARROW)) {
			square->Translate(0, 1);
		}
		if (e->ONKEY(esb::SPACE)) {
			square->fillMap(esb::BLACK);
		}
		if (e->ONKEY(esb::SHIFT)) {
			square->fillMap(esb::GREEN);
		}
		if (e->ONKEY(esb::ESCAPE)) {
			e->STOP();
		}

		e->RENDER(s);
	}
}

int main()
{
	Snake s;
	
	s.Run();

	return 0;
}