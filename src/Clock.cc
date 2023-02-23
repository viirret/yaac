#include "Clock.hh"
#include "Renderer.hh"

#include <sstream>

Clock::Clock(TTF_Font* font, Vec2i screenSize)
{
	// clock text is white, for now atleast
	SDL_Color white = { 255, 255, 255 };

	surface = TTF_RenderText_Solid(font, timeToText().c_str(), white);

	// convert into a texture
	texture = SDL_CreateTextureFromSurface(Renderer::get(), surface);

	// define clock size and position
	rect.w = 200;
	rect.h = 200;
	rect.x = screenSize.x / 2 - rect.w / 2;
	rect.y = screenSize.y / 2 - rect.h * 2;
}

Clock::~Clock()
{
	SDL_FreeSurface(surface);
	SDL_DestroyTexture(texture);
}

void Clock::render()
{
	SDL_RenderCopy(Renderer::get(), texture, nullptr, &rect);
}

std::string Clock::timeToText()
{
	std::stringstream ss;	

	// convert hours
	if(hours == 0)
		ss << "00";
	else 
		ss << hours;

	// add separator
	ss << ":";

	// convert minutes
	if(minutes == 0)
	 	ss << "00";
	else
		ss << minutes;

	return ss.str();
}
