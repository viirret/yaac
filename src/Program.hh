#ifndef YAAC_PROGRAM_HH
#define YAAC_PROGRAM_HH

#include "Window.hh"
#include "Color.hh"

#include <SDL2/SDL_ttf.h>

class Program
{
public:
	Program(int argc, char** argv);
private:
	// cmd arguments
	int argc;
	char** argv;
	Color bgColor;

	Window window;

	bool close = false;

	void update();
	void eventHandler();
	void render();

	// let's start by creating inputfield
	// this is too simple simple program to create abstraction for what we're creating
	TTF_Font* font;
	SDL_Surface* surface;
	SDL_Texture* texture;
	SDL_Rect inputField;
};

#endif
