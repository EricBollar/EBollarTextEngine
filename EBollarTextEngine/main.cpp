#include <iostream>
#include <windows.h>
#include <stdio.h>
#include "EBollarTextEngine.h"

void SetConsoleColour(WORD* Attributes, DWORD Colour)
{
	CONSOLE_SCREEN_BUFFER_INFO Info;
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(hStdout, &Info);
	*Attributes = Info.wAttributes;
	SetConsoleTextAttribute(hStdout, Colour);
}

void ResetConsoleColour(WORD Attributes)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Attributes);
}

int main()
{
	
	esb::EBollarTextEngine eng;
	eng.ConstructScene(50, 25);
	eng.ShowConsoleCursor(false);
	eng.MakeSpriteRect(5, 5, 2, 2, GREEN, "cube");
	eng.MakeSpriteRect(20, 20, 4, 1, RED, "wall");

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