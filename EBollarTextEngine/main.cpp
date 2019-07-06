#include <iostream>
#include <io.h>
#include <fcntl.h>
#include "EBollarTextEngine.h"

int main()
{
	_setmode(_fileno(stdout), _O_U16TEXT);
	
	esb::EBollarTextEngine eng;
	eng.ConstructScene(50, 25);
	eng.MakeSpriteRect(5, 5, 2, 1, eng.sR(4), "cube");
	std::vector<vec2D> sprMap;

	vec2D point1;
	point1.x = 0;
	point1.y = 0;
	point1.shade = eng.sR(4);
	sprMap.push_back(point1);
	vec2D point2;
	point2.x = 0;
	point2.y = 1;
	point2.shade = eng.sR(4);
	sprMap.push_back(point2);
	vec2D point3;
	point3.x = 0;
	point3.y = 2;
	point3.shade = eng.sR(4);
	sprMap.push_back(point3);
	vec2D point4;
	point4.x = 1;
	point4.y = 2;
	point4.shade = eng.sR(4);
	sprMap.push_back(point4);

	eng.MakeSprite(20, 20, sprMap, "wall");
	eng.ShowConsoleCursor(false);

	while (eng.gameLoop) {

		if (eng.OnKey(RIGHT)) {
			eng.MoveSprite(1, 0, "cube");
		}
		if (eng.OnKey(LEFT)) {
			eng.MoveSprite(-1, 0, "cube");
		}
		if (eng.OnKey(UP)) {
			eng.MoveSprite(0, -1, "cube");
		}
		if (eng.OnKey(DOWN)) {
			eng.MoveSprite(0, 1, "cube");
		}

		if (eng.CheckCollide("cube") == "wall") {
			eng.DelSprite("wall");
		}

		eng.GameLoop();
	}

	system("PAUSE");
	return 0;
}