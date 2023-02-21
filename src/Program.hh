#ifndef YAAC_PROGRAM_HH
#define YAAC_PROGRAM_HH

#include "Window.hh"
#include "Color.hh"
#include "Clock.hh"

#include <SDL2/SDL_ttf.h>

class Program
{
public:
	Program(int argc, char** argv);
private:
	// cmd arguments
	int argc;
	char** argv;

	Window window;
	TTF_Font* mainFont;

	Clock clock;

	Color bgColor;

	bool close = false;

	void update();
	void eventHandler();
	void render();
};

#endif
