#ifndef YAAC_COLOR_HH
#define YAAC_COLOR_HH

#include <SDL2/SDL.h>

struct Color 
{
	Color(int r, int g, int b, int a) : r(r), g(g), b(b), a(a) {}

	void black()
	{
		r = 0; g = 0; b = 0; a = 0;
	}

	void white()
	{
		r = 255; g = 255; b = 255; a = 0;
	}

	void change(int r, int g, int b, int a)
	{
		this->r = r; this->g = g; this->b = b; this->a = a;
	}

	SDL_Color getSDLColor()
	{
		return { static_cast<Uint8>(r), static_cast<Uint8>(g), static_cast<Uint8>(b), static_cast<Uint8>(a) };
	}

	int r, g, b, a;
};

#endif
