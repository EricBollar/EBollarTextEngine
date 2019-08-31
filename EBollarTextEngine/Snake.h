#include "EBollarTextEngine.h"
#include <vector>
#include <string>

class Snake : public esb::Scene {
	
private:
	esb::Engine *e = esb::Engine::Instance();
	esb::Scene game;

	int w;
	int h;

	esb::Sprite head;
	int headX = w / 2;
	int headY = h / 2;

	std::string snakeDir = "R";

public:
	Snake();
	void MoveSnake();
	void HandleEvents();
	void Run();
	void GetInputs();
};