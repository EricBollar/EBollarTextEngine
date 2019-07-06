#include <string>
#include <vector>
#include <array>
#include <io.h>
#include <fcntl.h>
#include <thread>

enum keyCode {
	RIGHT,
	LEFT,
	UP,
	DOWN
};

enum color {
	BLUE,
	RED,
	GREEN
};

struct vec2D { // used only by Sprites
	int x, y;
	wchar_t shade;
};

struct Sprite {
	std::string name = "";
	int x, y;
	std::vector<vec2D> map;
	color c;
};

struct Scene {
	int w, h;

	std::vector<wchar_t> x;
	std::vector<std::vector<wchar_t> > arr2D;

	std::vector<wchar_t> filler;
	std::vector<std::vector<wchar_t> > prevFrame;

	std::vector<Sprite> sprites;
};

namespace esb {
	class EBollarTextEngine {
	private:
		Scene s;
		std::vector<vec2D> CompareFrames(std::vector<std::vector<wchar_t> >& frame1, std::vector<std::vector<wchar_t> >& frame2);
		int rRate = 100;
		wchar_t baseFill = L' ';
		bool m_Running = false;
		bool canMove = true;
		void SpriteCollate();
		int FindSpriteWithName(std::string name);
		void setConsoleColour(unsigned short colour);
		void cls();
		void setCursorPosition(int x, int y);
	public:
		void ShowConsoleCursor(bool ysno);
		bool gameLoop = m_Running;
		void GameLoop();
		void Stop();
		void ConstructScene(int width, int height);
		void FillScene(wchar_t shade);
		void PrintScene();
		wchar_t sR(int i);
		void PaintPix(int x, int y, wchar_t shade);
		void MakeSprite(int x, int y, std::vector<vec2D> m, std::string name);
		void MakeSpriteRect(int x, int y, int w, int h, wchar_t shade, std::string name);
		void DelSprite(std::string name);
		void ClearSprites();
		void MoveSprite(int xC, int yC, std::string name);
		bool OnKey(keyCode key);
		void RefreshRate(int milliseconds);
		std::string CheckCollide(std::string name);
	};
}