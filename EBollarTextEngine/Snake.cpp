#include "Snake.h"

Snake::Snake() {
	eng.ConstructScene(60, 30);
	eng.MakeSpriteRect(5, 5, 2, 2, esb::GREEN, "head");
	eng.ShowConsoleCursor(true);

	GameLoop();
}

void Snake::GameLoop() {
	while (eng.gameLoop) {
		if (eng.OnKey(esb::DOWN)) {
			eng.MoveSprite(0, 1, "head");
		}
		if (eng.OnKey(esb::UP)) {
			eng.MoveSprite(0, -1, "head");
		}
		if (eng.OnKey(esb::RIGHT)) {
			eng.MoveSprite(1, 0, "head");
		}
		if (eng.OnKey(esb::LEFT)) {
			eng.MoveSprite(-1, 0, "head");
		}
		
		eng.GameLoop();
	}
}