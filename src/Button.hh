#ifndef YAAC_BUTTON_HH
#define YAAC_BUTTON_HH

#include "Color.hh"
#include "Config.hh"

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#include <functional>

class Button
{
public:
    Button(const SDL_Rect& rect, TTF_Font* font, const std::string& text, const std::function<void(Button&)>& click, Config& config);
    ~Button();

    Button(Button&& button);

    void update();

    SDL_Rect rect;

    bool isPressed = false;
    bool draw = true;

    const std::function<void(Button&)> click;
private:
    // the actual button
    SDL_Texture* buttonTexture;

    // text inside the button
    SDL_Texture* textTexture;

    // button colors
    Color buttonColor;
    Color textColor;
    Color blinkColor;

	bool playClickSound;
};

#endif
