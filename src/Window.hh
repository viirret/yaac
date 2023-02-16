#include "Renderer.hh"
#include "Vector2.hh"

#include <SDL2/SDL.h>

#include <string>
#include <iostream>

class Window
{
public:
	Window(const std::string& title);
	~Window();

	bool valid() { return window; }

	Vec2i swapSize(Vec2i newSize);
	Vec2i getSize() { return size; }

private:
	Vec2i size;

	SDL_Window* window;
	SDL_Renderer* renderer;
};
