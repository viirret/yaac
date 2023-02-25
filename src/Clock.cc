#include "Clock.hh"
#include "Renderer.hh"

#include <SDL2/SDL_surface.h>
#include <sstream>

Clock::Clock(TTF_Font* font, Vec2i screenSize)
	: font(font)
{
	// define clock size and position
	rect.w = 200;
	rect.h = 200;
	rect.x = screenSize.x / 2 - rect.w / 2;
	rect.y = screenSize.y / 2 - rect.h * 2;

	updateTexture();
}

Clock::~Clock()
{
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
	if(hours < 10)
		ss << "0" << hours;
	else
		ss << hours;

	// add separator
	ss << ":";

	// convert minutes
	if(minutes < 10)
		ss << "0" << minutes;
	else
		ss << minutes;

	return ss.str();
}

void Clock::updateTexture()
{
	// create surface
	SDL_Surface* surface = TTF_RenderText_Solid(font, timeToText().c_str(), white);

	// update texture
	texture = SDL_CreateTextureFromSurface(Renderer::get(), surface);
}

void Clock::addHour()
{
	if(hours >= 23)
		hours = 0;
	else 
		hours++;
	
	updateTexture();
}

void Clock::removeHour()
{
	if(hours <= 0)
		hours = 23;
	else
		hours--;

	updateTexture();
}

void Clock::addMinute()
{
	if(minutes >= 59)
		minutes = 0;
	else
		minutes++;
	
	updateTexture();
}

void Clock::removeMinute()
{
	if(minutes <= 0)
		minutes = 59;
	else
	 	minutes--;
	
	updateTexture();
}

