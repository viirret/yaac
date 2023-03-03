#include "Button.hh"
#include "Renderer.hh"

Button::Button(SDL_Rect rect, TTF_Font* font, const std::string& text, const std::function<void(Button&)>& click) 
	: 	rect(rect), click(click), 
		mainColor(255, 0, 0, 255),
		textColor(255, 255, 255, 255),
		blinkColor(0, 0, 0, 255)
{
	// create button surface
	SDL_Surface* buttonSurface = SDL_CreateRGBSurface(0, rect.w, rect.h, 32, 0, 0, 0, 0);

	// mainColor
	SDL_FillRect(buttonSurface, nullptr, SDL_MapRGB(buttonSurface->format, mainColor.r, mainColor.g, mainColor.b));

    buttonTexture = SDL_CreateTextureFromSurface(Renderer::get(), buttonSurface);

	// create text inside the button
	SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), textColor.getSDLColor());
	textTexture = SDL_CreateTextureFromSurface(Renderer::get(), textSurface);

	SDL_FreeSurface(buttonSurface);
	SDL_FreeSurface(textSurface);
}

Button::~Button()
{
	if(textTexture) 
		SDL_DestroyTexture(textTexture);
	if(buttonTexture)
		SDL_DestroyTexture(buttonTexture);
}

Button::Button(Button&& button) 
	: 	click(std::move(button.click)), isPressed(button.isPressed),
		buttonTexture(button.buttonTexture), textTexture(button.textTexture),
		mainColor(255, 0, 0, 255),
		textColor(255, 255, 255, 255),
		blinkColor(0, 255, 0, 255)
{
	rect.x = button.rect.x;
	rect.y = button.rect.y;
	rect.w = button.rect.w;
	rect.h = button.rect.h;

	button.textTexture = nullptr;
	button.buttonTexture = nullptr;
}

void Button::render()
{
	// button
	SDL_RenderCopy(Renderer::get(), buttonTexture, nullptr, &rect);

	// text inside it
	SDL_RenderCopy(Renderer::get(), textTexture, nullptr, &rect);

	if(isPressed)
	{
	    // change the color for one frame
		SDL_SetRenderDrawColor(Renderer::get(), blinkColor.r, blinkColor.g, blinkColor.b, blinkColor.a);
        SDL_RenderFillRect(Renderer::get(), &rect);

		// go back to normal
		isPressed = false;
	}
}

