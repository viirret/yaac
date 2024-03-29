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
    // Constructor.
    Button(const SDL_Rect& rect, TTF_Font* font, const std::string& text, const std::function<void(Button&)>& click, Config& config);

    // Destructor.
    ~Button();

    // Copy Constructor.
    Button(Button&& button);

    // Main update method.
    void update();

    // SDL rectange.
    SDL_Rect rect;

    bool isPressed = false;
    bool draw = true;

    const std::function<void(Button&)> click;

    void setClickSound(bool state)
    {
        playClickSound = state;
    }

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
