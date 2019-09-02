#include "EBollarTextEngine.h"
#include <vector>
#include <string>

class Snake : public esb::Scene {
	
private:
	esb::Engine *e = esb::Engine::Instance();
	esb::Scene game;
	esb::Scene menu;
	esb::Scene controls;
	esb::Scene death;

	esb::Scene* currScene = &game;

	std::vector<esb::Sprite*> snakeBody;

	esb::Sprite* head;
	esb::Sprite* apple;
	esb::Sprite* cursor;
	esb::Sprite* playGame;
	esb::Sprite* returnToMenu;

	int w = 20;
	int h = 20;
	int cursorPos = 0;

	int headX = w / 2;
	int headY = h / 2;

	std::string snakeDir = "R";
	std::string prevDir = "R";

public:
	Snake();
	void MoveCursor();
	void SpawnBody();
	void SpawnApple();
	void MoveSnake();
	void HandleEvents();
	void Run();
	void GetInputs();
};