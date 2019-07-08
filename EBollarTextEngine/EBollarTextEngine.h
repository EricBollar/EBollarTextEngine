#include <string>
#include <vector>
#include <array>
#include <thread>
#include <Windows.h>

namespace esb {

	enum direction {
		RIGHT,
		LEFT,
		UP,
		DOWN,
		W,
		S
	};

	enum color {
		BLUE,
		RED,
		GREEN,
		YELLOW,
		PINK,
		LIGHTBLUE,
		BLACK,
		WHITE
	};

	struct Pixel {
		color c;
		color textC;
		char t = ' ';
	};

	struct vec2D { // used by Sprites
		int x, y;
		color c;
		color textC;
		char t = ' '; // if i want to print text i can override the default "space" char but otherwise i dont wanna deal with it in my non-text sprites
	};

	struct Sprite {
		std::string name = "";
		int x, y;
		std::vector<vec2D> map;
	};

	struct Scene {
		int w, h;

		std::vector<Pixel> x;
		std::vector<std::vector<Pixel> > arr2D;

		std::vector<Pixel> filler;
		std::vector<std::vector<Pixel> > prevFrame;

		std::vector<Sprite> sprites;
	};

	class EBollarTextEngine {
	private:
		Scene s;
		std::vector<vec2D> CompareFrames(std::vector<std::vector<Pixel> >& frame1, std::vector<std::vector<Pixel> >& frame2);
		int rRate = 150;
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
		void CheckSpriteCOO();
	public:
		int getPosX(std::string name);
		int getPosY(std::string name);
		void PushSprite(Sprite a);
		DWORD c(color c);
		void ShowConsoleCursor(bool ysno);
		bool gameLoop = m_Running;
		void Render();
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
		void setX(int x, std::string a);
		void setY(int y, std::string a);
		void SetBackground(esb::color c);
		void MakeText(int x, int y, color c, std::string string, std::string name);
		void setText(std::string string, std::string name);
		std::string CheckCollide(std::string name);
	};
}