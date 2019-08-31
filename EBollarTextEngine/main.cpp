#include <iostream>
#include <windows.h>
#include <stdio.h>
#include "Snake.h"

void Demo() {
	esb::Engine* e = esb::Engine::Instance();
	esb::Scene s;

	s.ConstructScene(20, 15);
	s.SetBackground(esb::LIGHTBLUE);

	esb::Sprite* square = s.MakeSpriteRect(8, 4, 1, 1, esb::RED, "square");
	e->SETREFRESHRATE(4);

	while (s.RUNNING) {
		if (e->ONKEY(esb::RIGHTARROW)) {
			square->Translate(1, 0);
		}
		if (e->ONKEY(esb::SPACE)) {
			square->fillMap(esb::BLACK);
		}

		e->RENDER(s);
	}
}

int main()
{
	Demo();

	system("PAUSE");
	return 0;
}