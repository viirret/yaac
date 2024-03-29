#ifndef YAAC_CLOCK_HH
#define YAAC_CLOCK_HH

#include "Button.hh"
#include "Color.hh"
#include "Config.hh"
#include "Opts.hh"
#include "Vector2.hh"

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#include <chrono>

// State machine for clock.
enum class ClockState
{
    NOT_SET,
    RUNNING_TIMER,
    RINGING,
    SHOW_CLOCK
};

class Clock
{
public:
    // Constructor.
    Clock(TTF_Font* font, Vec2i screenSize, Color* bgColor, const Config& config, const Opts& opts);

    // Destructor.
    ~Clock();

    // current state of this clock
    ClockState state;

    void render();

    // clock modifier functions
    void addHour();
    void removeHour();
    void addMinute();
    void removeMinute();

    void startTimer();
    void updateTimer();

    // update main texture
    void updateTexture();

    // play song when ringing
    void loopSong();

    // just show normal clock after ringing is done
    void showTime();

private:
    // sound that timer makes when done
    Mix_Music* sound = nullptr;

    TTF_Font* font;

    // texture for the main clock
    SDL_Texture* texture;

    // how much time is left until timer is done
    SDL_Texture* timeLeftTex = nullptr;

    // rect for the clock and timer
    SDL_Rect rect, timeLeftRect;

    // clock color (2 textures)
    Color clockColor;

    // backgroundColor in Program
    Color* bgColor;
    Color originalBgColor;

    // how much time is left
    std::chrono::duration<double> timeLeft;
    std::chrono::hours::rep hoursLeft;
    std::chrono::minutes::rep minutesLeft;
    std::chrono::seconds::rep secondsLeft;

    // main texture:
    // values
    int hours;
    int minutes;
    int seconds = 0;

    // timer texture:
    std::string timeToText();
    std::string createTimeString();
    void updateClockValues();
    void updateTimerTexture();

    // copy of options
    Opts opts;
};

#endif
