#include "EBollarTextEngine.h"
#include <vector>
#include <string>

class Snake : public esb::Scene {
	
private:
	esb::Engine *e = esb::Engine::Instance();
	esb::Scene game;
	esb::Scene menu;
	esb::Scene controls;

	std::vector<esb::Sprite*> snakeBody;

	esb::Sprite* head;
	esb::Sprite* apple;

	int w = 20;
	int h = 20;

	int headX = w / 2;
	int headY = h / 2;

	std::string snakeDir = "R";

public:
	Snake();
	void SpawnBody();
	void SpawnApple();
	void MoveSnake();
	void HandleEvents();
	void Run();
	void GetInputs();
};