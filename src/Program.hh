#ifndef YAAC_PROGRAM_HH
#define YAAC_PROGRAM_HH

#include "Window.hh"
#include "Color.hh"
#include "Clock.hh"
#include "Button.hh"

#include <SDL2/SDL_ttf.h>

#include <vector>
#include <memory>

class Program
{
public:
	Program(int argc, char** argv);
private:
	// cmd arguments
	int argc;
	char** argv;

	// window object that starts SDL
	Window window;

	// font used throughout this program
	TTF_Font* mainFont;

	Clock clock;

	// backgroundcolor
	Color bgColor;

	// there are multiple buttons in our alarm clock
	std::vector<std::unique_ptr<Button>> buttons;

	bool close = false;

	void update();
	void eventHandler();
	void render();
};

#endif
