#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <string>
#include <functional>

class Button
{
public:
	Button(SDL_Rect rect, TTF_Font* font, const std::string& text, std::function<void()> click);
	~Button();
	void render();
	SDL_Rect rect;
	std::function<void()> click;

	bool isPressed = false;

private:
	// the actual button
	SDL_Surface* buttonSurface;
	SDL_Texture* buttonTexture;
	
	// text inside the button
	SDL_Surface* textSurface;
	SDL_Texture* textTexture;
};
