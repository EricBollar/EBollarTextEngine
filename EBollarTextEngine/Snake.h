#include "EBollarTextEngine.h"
#include <vector>
#include <string>

class Snake : public esb::Scene {
	
private:
	esb::Engine *e = esb::Engine::Instance();
	esb::Scene gameS;
	esb::Scene menuS;
	esb::Scene deathS;

	std::vector<esb::Sprite*> snakeBody;

	esb::Sprite* head;
	esb::Sprite* apple;
	esb::Sprite* playGameText;
	esb::Sprite* returnToMenuText;
	esb::Sprite* title;
	esb::Sprite* titleT;

	int w = 20;
	int h = 20;
	int cursorPos = 0;

	int headX = w / 2;
	int headY = h / 2;
	bool wait = false;

	std::string snakeDir = "R";
	std::string prevDir = "R";

public:
	Snake();
	void SpawnBody();
	void SpawnApple();
	void MoveSnake();
	void HandleEvents();
	void Run();
	void GetInputs();

	void SetupGame();
	void resetVars();
	void createSprites();
};