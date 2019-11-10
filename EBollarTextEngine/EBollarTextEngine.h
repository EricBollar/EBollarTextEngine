/*
	Windows Console Game/Graphics Engine
				made by Eric S. Bollar
*/

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
		std::string t = ""; // text displayed (automatically a space because it cannot be blank to print an empty pix)
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
		void SpriteCollate(); // puts the sprites onto the array
	public:
		Sprite* FindSprite(std::string name, int id = -1);
		int getW();
		int getH();
		esb::color getBackgroundColor();
		void AddSprite(Sprite a); // adds sprite a to the scene
		void ConstructScene(int width, int height, color backgroundColor); // creates the scene
		void FillScene(color c); // fills the frame vector with c
		esb::Sprite* MakeSprite(int x, int y, std::vector<esb::Pixel> m, std::string name, int id = -1); // make a sprite with custom map
		esb::Sprite* MakeSpriteRect(int x, int y, int w, int h, color c, std::string name, int id = -1); // make a rectangle sprite
		void DelSprite(Sprite s); // removes sprite s
		void ClearSprites(); // removes all the sprites
		esb::Sprite* MakeText(int x, int y, color c, std::string string, std::string name, int id = -1); // makes text
		bool CheckSpriteCollide(esb::Sprite* sprite1, esb::Sprite* sprite2); // returns true if any pixel of sprite1 is overlapping any pixel of sprite2
		std::vector<std::vector<Pixel> > getFrame(); // used by engine to get the next frame
		std::vector<Pixel> getDiffs(); // used by engine to get the differences between frames
		void Process(); // processes everything for next frame
		void clearDiffs(); // forces the engine to print the entire frame and not just diffs
	};

	class Engine { // singleton class to prevent multiple engines and possible breakage :)
	public:
		static Engine* Instance();
	private:
		Engine() {}; // all constructors set to private so that instantiation can only be made using Instance()
		Engine(Engine const&) {}; // i gotta put curly brackets to disregard unresolved externals
		Engine& operator=(Engine const&) {};

		bool running = true; // for game loop
		Scene* currScene; // ptr for current scene
		int rRate; // # of milliseconds between each frame
		WORD Attributes = 0;
		std::vector<std::vector<Pixel> > prevFrame; // array of pixels for prev frame
		void PrintScene(); // Prints the current frame
	public:
		bool RUNNING(); // Bool for the game loop
		void STOP(); // Stops program
		void LOAD(Scene* s); // Changes the scene that is rendered
		esb::Scene* GETSCENE(); // Returns the scene that is currently loaded
		void SETREFRESHRATE(int milliseconds); // Sets the number of milliseconds between each frame
		bool ONKEY(keyCode key); // Returns if a certain key is pressed
		DWORD GETCOLOR(color c); // Converts color enum to DWORD type
		void RENDER(); // Displays the next frame of the current scene
	};
}