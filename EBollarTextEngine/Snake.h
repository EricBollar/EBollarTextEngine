#include "EBollarTextEngine.h"
#include <vector>

class Snake : public esb::Scene {
	struct point {
		int x, y;
	};

private:
	esb::Scene s;
	esb::Engine* e = esb::Engine::Instance();
	std::vector<point> body;
	esb::direction movingDir = esb::RIGHT;
	int wait = 0;
	int w = 57; int h = 27;
public:
	Snake();
	void Run();
	void GrowBody();
	void Movement();
	void SpawnApple();
};