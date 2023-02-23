#include "Vector2.hh"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <string>

class Clock
{
public:
	Clock(TTF_Font* font, Vec2i screenSize);
	~Clock();
	void render();
private:
	SDL_Surface* surface;
	SDL_Texture* texture;
	SDL_Rect rect;

	std::string timeToText();

	int hours = 0;
	int minutes = 0;
};
