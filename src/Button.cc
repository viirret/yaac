#include "Button.hh"
#include "Renderer.hh"
#include "Util.hh"

// default colors if config file fails
#define buttonColorDefault Color(255, 0, 0, 255)
#define buttonTextDefault Color(255, 255, 255, 255)
#define buttonBlinkDefault Color(0, 255, 0, 255)

Button::Button(const SDL_Rect& rect, TTF_Font* font, const std::string& text, const std::function<void(Button&)>& click, Config& config)
    : rect(rect), click(click),
	  buttonColor(Util::readHexColor(config.get("buttonColor"), buttonColorDefault)),
      textColor(Util::readHexColor(config.get("buttonText"), buttonTextDefault)),
      blinkColor(Util::readHexColor(config.get("buttonBlink"), buttonBlinkDefault))
{
    // create button surface
    SDL_Surface* buttonSurface = SDL_CreateRGBSurface(0, rect.w, rect.h, 32, 0, 0, 0, 0);

    // mainColor
    SDL_FillRect(buttonSurface, nullptr, SDL_MapRGB(buttonSurface->format, buttonColor.r, buttonColor.g, buttonColor.b));

    buttonTexture = SDL_CreateTextureFromSurface(Renderer::get(), buttonSurface);

    // create text inside the button
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), textColor.getSDLColor());
    textTexture = SDL_CreateTextureFromSurface(Renderer::get(), textSurface);

    SDL_FreeSurface(buttonSurface);
    SDL_FreeSurface(textSurface);
}

Button::~Button()
{
    if (textTexture)
        SDL_DestroyTexture(textTexture);
    if (buttonTexture)
        SDL_DestroyTexture(buttonTexture);
}

Button::Button(Button&& button)
    : click(std::move(button.click)), isPressed(button.isPressed),
      buttonTexture(button.buttonTexture), textTexture(button.textTexture)
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

    if (isPressed)
    {
        // change the color for one frame
        SDL_SetRenderDrawColor(Renderer::get(), blinkColor.r, blinkColor.g, blinkColor.b, blinkColor.a);
        SDL_RenderFillRect(Renderer::get(), &rect);

        // go back to normal
        isPressed = false;
    }
}
