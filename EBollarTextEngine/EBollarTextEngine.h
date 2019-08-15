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
		int x, y; // mainly just used for spritemaps, when added to frame this is left blank
		color c; // color
		color textColor; // color of text
		char t = ' '; // text displayed (automatically a space because it cannot be blank to print an empty pix)
	};

	struct Sprite {
		std::string name = ""; // name of instance
		int x, y; // pos
		std::vector<Pixel> map; // visual
		int id; // internal random value or can be manually set by user if more than one of certain name
	};

	struct Scene {
		int w, h; // dimensions

		std::vector<Pixel> row; // the row arr2D uses to iterate through frame
		std::vector<std::vector<Pixel> > arr2D; // array of pixels for next frame

		std::vector<Pixel> filler; // the row that prevFrame uses to iterate through frame
		std::vector<std::vector<Pixel> > prevFrame; // array of pixels for prev frame

		std::vector<Sprite> sprites; // the sprites
	};

	class EBollarTextEngine {
	private:
		Scene s;
		std::vector<esb::Pixel> CompareFrames(std::vector<std::vector<esb::Pixel> >& frame1, std::vector<std::vector<esb::Pixel> >& frame2);
		int rRate = 150;
		WORD Attributes = 0;
		color baseColor = BLUE;
		bool canPress = true;
		void SpriteCollate();
		int FindSpriteIndex(std::string name, int id = -1);
		void setConsoleColour(WORD* Attributes, DWORD Colour);
		void ResetConsoleColour(WORD Attributes);
		void cls();
		void setCursorPosition(int x, int y);
		//void CheckSpriteCOO();
	public:
		bool RUNNING = false;
		int getPosX(std::string name, int id = -1);
		int getPosY(std::string name, int id = -1);
		void AddSprite(Sprite a);
		DWORD c(color c);
		void ShowConsoleCursor(bool ysno);
		void Render();
		void Stop();
		void ConstructScene(int width, int height);
		void FillScene(color c);
		void PrintScene();
		void MakeSprite(int x, int y, std::vector<esb::Pixel> m, std::string name, int id = -1);
		void MakeSpriteRect(int x, int y, int w, int h, color c, std::string name, int id = -1);
		void DelSprite(std::string name, int id = -1);
		void ClearSprites();
		void MoveSprite(int xC, int yC, std::string name, int id = -1);
		bool OnKey(direction key);
		void setRefreshRate(int milliseconds);
		void setX(int x, std::string name, int id = -1);
		void setY(int x, std::string name, int id = -1);
		void SetBackground(esb::color c);
		void MakeText(int x, int y, color c, std::string string, std::string name, int id = -1);
		void setText(std::string string, std::string name, int id = -1);
		std::string CheckSpriteCollide(std::string name, int id = -1);
	};
}