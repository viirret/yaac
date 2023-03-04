#include "Renderer.hh"

namespace Renderer
{

static SDL_Renderer* renderer;	

void set(SDL_Renderer* ren) { renderer = ren; }

SDL_Renderer* get() { return renderer; }

void render() 
{
	SDL_RenderPresent(renderer); 
}

void clear()
{
	SDL_RenderClear(renderer);
}

void setColor(Color color)
{
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}

}
