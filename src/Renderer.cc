#include "Renderer.hh"

namespace Renderer
{
	static SDL_Renderer* renderer;	

	void set(SDL_Renderer* ren) { renderer = ren; }

	SDL_Renderer* get() { return renderer; }

	void render() 
	{
		SDL_RenderClear(renderer);
		SDL_RenderPresent(renderer); 
	}

	void renderBackground(Color color)
	{
		SDL_SetRenderDrawColor(renderer, color.R, color.G, color.B, color.A);
	}
}
