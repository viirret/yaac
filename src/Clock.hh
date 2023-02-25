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
	
	// clock modifier functions
	void addHour();
	void removeHour();
	void addMinute();
	void removeMinute();

private:
	TTF_Font* font;
	SDL_Texture* texture;
	SDL_Rect rect;

	// clock text is white, for now atleast
	SDL_Color white = { 255, 255, 255 };

	std::string timeToText();
	void updateTexture();

	int hours = 0;
	int minutes = 0;
};
