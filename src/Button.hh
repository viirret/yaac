#ifndef YAAC_BUTTON_HH
#define YAAC_BUTTON_HH

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <string>
#include <functional>

class Button
{
public:
	Button(SDL_Rect rect, TTF_Font* font, const std::string& text, const std::function<void(Button&)>& click);
	~Button();
	
	Button(Button&& button);

	void render();

	SDL_Rect rect;
	const std::function<void(Button&)> click;

	bool isPressed = false;

private:
	// the actual button
	SDL_Texture* buttonTexture;
	
	// text inside the button
	SDL_Texture* textTexture;
};

#endif
