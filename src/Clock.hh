#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class Clock
{
public:
	Clock(TTF_Font* font);
	~Clock();
	void render();
private:
	SDL_Surface* surface;
	SDL_Texture* texture;
	SDL_Rect rect;
};
