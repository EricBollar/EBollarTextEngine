#include <string>
#include <vector>
#include <array>
#include <thread>
#include <Windows.h>

enum direction {
	RIGHT,
	LEFT,
	UP,
	DOWN
};

enum color {
	BLUE,
	RED,
	GREEN,
	NOCOL
};

struct vec2D { // used only by Sprites
	int x, y;
	color c;
};

struct Sprite {
	std::string name = "";
	int x, y;
	std::vector<vec2D> map;
};

struct Scene {
	int w, h;

	std::vector<color> x;
	std::vector<std::vector<color> > arr2D;

	std::vector<color> filler;
	std::vector<std::vector<color> > prevFrame;

	std::vector<Sprite> sprites;
};

namespace esb {
	class EBollarTextEngine {
	private:
		Scene s;
		std::vector<vec2D> CompareFrames(std::vector<std::vector<color> >& frame1, std::vector<std::vector<color> >& frame2);
		int rRate = 100;
		WORD Attributes = 0;
		color baseColor = BLUE;
		bool m_Running = false;
		bool canMove = true;
		void SpriteCollate();
		int FindSpriteWithName(std::string name);
		void setConsoleColour(WORD* Attributes, DWORD Colour);
		void ResetConsoleColour(WORD Attributes);
		void cls();
		void setCursorPosition(int x, int y);
	public:
		DWORD c(color c);
		void ShowConsoleCursor(bool ysno);
		bool gameLoop = m_Running;
		void GameLoop();
		void Stop();
		void ConstructScene(int width, int height);
		void FillScene(color c);
		void PrintScene();
		void PaintPix(int x, int y, color c);
		void MakeSprite(int x, int y, std::vector<vec2D> m, std::string name);
		void MakeSpriteRect(int x, int y, int w, int h, color c, std::string name);
		void DelSprite(std::string name);
		void ClearSprites();
		void MoveSprite(int xC, int yC, std::string name);
		bool OnKey(direction key);
		void RefreshRate(int milliseconds);
		std::string CheckCollide(std::string name);
	};
}