#include <string>
#include <vector>
#include <array>
#include <thread>
#include <Windows.h>
#include <iostream>

namespace esb {

	enum keyCode {
		RIGHTARROW,
		LEFTARROW,
		UPARROW,
		DOWNARROW,
		A,
		B,
		C,
		D,
		E,
		F,
		G,
		H,
		I,
		J,
		K,
		L,
		M,
		N,
		O,
		P,
		Q,
		R,
		S,
		T,
		U,
		V,
		W,
		X,
		Y,
		Z,
		RIGHTMOUSEBUTTON,
		LEFTMOUSEBUTTON,
		MIDDLEMOUSEBUTTON,
		BACKSPACE,
		TAB,
		ENTER,
		SHIFT,
		ESCAPE,
		SPACE
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

	class Sprite {
	public:
		Sprite() = default;
	private:
		int index; // the index in the scene's sprites vec
		int w, h;
		std::string name = ""; // name of instance
		int x, y; // pos
		std::vector<Pixel> map; // visual
		int id; // internal random value or can be manually set by user if more than one of certain name
	public:
		void Translate(int xChange, int yChange);
		void fillMap(color c);
		void setMap(std::vector<Pixel> map);
		void setText(std::string t);
		void setX(int x);
		void setY(int y);
		int getX();
		int getY();
		std::vector<Pixel> &getMap();
		std::string getName();
		int getID();
		void setName(std::string n);
		void setID(int i);
		void setW(int w);
		void setH(int h);
		void setIndex(int i);
		int getIndex();
	};

	class Scene {
	private:
		
		int w, h; // dimensions

		std::vector<Pixel> row; // the row arr2D uses to iterate through frame
		std::vector<std::vector<Pixel> > arr2D; // array of pixels for next frame

		std::vector<Pixel> filler; // the row that prevFrame uses to iterate through frame
		std::vector<std::vector<Pixel> > prevFrame; // array of pixels for prev frame

		std::vector<Sprite* > sprites; // the sprites

		std::vector<esb::Pixel> CompareFrames(std::vector<std::vector<esb::Pixel> >& frame1, std::vector<std::vector<esb::Pixel> >& frame2);
		int rRate = 80;
		WORD Attributes = 0;
		color baseColor = BLUE;
		bool canPress = true;
		void SpriteCollate();
	public:
		Sprite* FindSprite(std::string name, int id = -1);
		int getW();
		int getH();
		void AddSprite(Sprite a);
		void ConstructScene(int width, int height, color backgroundColor);
		void FillScene(color c);
		void MakeSprite(int x, int y, std::vector<esb::Pixel> m, std::string name, Sprite* ptr, int id = -1);
		esb::Sprite* MakeSpriteRect(int x, int y, int w, int h, color c, std::string name, int id = -1);
		void DelSprite(Sprite s);
		void ClearSprites();
		void MakeText(int x, int y, color c, std::string string, std::string name, Sprite* ptr, int id = -1);
		bool CheckSpriteCollide(esb::Sprite* sprite1, esb::Sprite* sprite2);
		std::vector<std::vector<Pixel> > getFrame();
		std::vector<Pixel> getDiffs();
		void Process();
	};

	class Engine { // singleton class to prevent multiple engines and possible breakage :)
	public:
		static Engine* Instance();
	private:
		Engine() {}; // all constructors set to private so that instantiation can only be made using Instance()
		Engine(Engine const&) {}; // i gotta put curly brackets to disregard unresolved externals
		Engine& operator=(Engine const&) {};
		bool running = true;

		Scene currScene;
		int rRate;
		WORD Attributes = 0;
		std::vector<std::vector<Pixel> > prevFrame; // array of pixels for prev frame

		void PrintScene();
	public:
		bool RUNNING();
		void STOP();
		void SETREFRESHRATE(int milliseconds);
		bool ONKEY(keyCode key);
		DWORD GETCOLOR(color c);
		void RENDER(Scene s);
	};
}