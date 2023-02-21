#include "Clock.hh"
#include "Renderer.hh"

Clock::Clock(TTF_Font* font)
{
	// clock text is white, for now atleast
	SDL_Color white = { 255, 255, 255 };

	surface = TTF_RenderText_Solid(font, "00:00", white);

	// convert into a texture
	texture = SDL_CreateTextureFromSurface(Renderer::get(), surface);

	// define clock position
	rect.x = 0;
	rect.y = 0;
	rect.w = 100;
	rect.h = 100;
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
