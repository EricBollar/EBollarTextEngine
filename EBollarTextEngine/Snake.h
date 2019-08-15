#include "EBollarTextEngine.h"
#include <vector>

class Snake : public esb::EBollarTextEngine{
	struct point {
		int x, y;
	};

private:
	esb::EBollarTextEngine eng;
	std::vector<point> body;
	esb::direction movingDir = esb::RIGHT;
	int wait = 0;
	int w = 57; int h = 27;
public:
	Snake();
	void GameLoop();
	void GrowBody();
	void Movement();
	void SpawnApple();
};