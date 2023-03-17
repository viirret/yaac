#include "Program.hh"
#include "Button.hh"
#include "Clock.hh"
#include "Renderer.hh"
#include "Settings.hh"
#include "Util.hh"

#include <SDL2/SDL_mixer.h>

#include <ctime>
#include <sstream>
#include <string>

// button size configuration
#define wsize window.getSize()
#define bsize 50

// default configuration (no configuration file)
#define defaultBackgroundColor Color(255, 255, 255, 255)

Program::Program(int argc, char** argv)
    : opts(argc, argv),
      config(Settings::CONFIG, '='),
      window(""),
      mainFont(TTF_OpenFont((Settings::FONTDIR).c_str(), 24)),
      bgColor(Util::readHexColor(config.get("color"), defaultBackgroundColor)),
      clock(mainFont, wsize, &bgColor, config)
{
    buttons.emplace_back(
        // position and size for the button
        SDL_Rect{wsize.x / 2 - wsize.x / 8, wsize.y / 2 - wsize.y / 6, bsize, bsize},

        // font and text
        mainFont, "+",

        // what happens when clicked
        [this](Button& button)
        {
            // currently flicks color, could add something like sound later if want
            button.isPressed = true;

            // first clock function
            clock.addHour();
        },
        config);

    buttons.emplace_back(
        SDL_Rect{wsize.x / 2 - wsize.x / 8, wsize.y / 2, bsize, bsize},
        mainFont, "-",
        [this](Button& button)
        {
            button.isPressed = true;
            clock.removeHour();
        },
        config);

    buttons.emplace_back(
        SDL_Rect{wsize.x / 2 + wsize.x / 8 - bsize, wsize.y / 2 - wsize.y / 6, bsize, bsize},
        mainFont, "+",
        [this](Button& button)
        {
            button.isPressed = true;
            clock.addMinute();
        },
        config);

    buttons.emplace_back(
        SDL_Rect{wsize.x / 2 + wsize.x / 8 - bsize, wsize.y / 2, bsize, bsize},
        mainFont, "-",
        [this](Button& button)
        {
            button.isPressed = true;
            clock.removeMinute();
        },
        config);

    buttons.emplace_back(
        // TODO this is not properly centered, I'll do this when I have time for styling
        SDL_Rect{wsize.x / 2 - bsize * 2, wsize.y / 2 + wsize.y / 6, bsize * 4, bsize},
        mainFont, "Start alarm",

        [this](Button& button)
        {
            button.isPressed = true;
            startClock();
        },
        config);

    // create button that can test sounds
    buttons.emplace_back(
        SDL_Rect{wsize.x - wsize.x / 6, wsize.y / 8, bsize * 4, bsize},
        mainFont, "Test sounds",
        [](Button& button)
        {
            button.isPressed = true;

            // create clicksound
            Mix_Music* click = Mix_LoadMUS((Settings::DEEPINSONGDIR + "complete-print.wav").c_str());

            // file not found
            if (!click)
            {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Cannot find clicksound: %s", Mix_GetError());
            }

            // play clicksound
            if (Mix_PlayMusic(click, 0) == -1)
            {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Cannot play clicksound: %s", Mix_GetError());
            }
        },
        config);

    // the reset button
    buttons.emplace_back(
        SDL_Rect{wsize.x / 2 - bsize * 2, wsize.y / 10, bsize * 4, bsize},
        mainFont, "Reset clock",
        [this](Button& button)
        {
            button.isPressed = true;
            button.draw = false;

            // update state
            clock.state = ClockState::NOT_SET;
        },
        config);

    // dont't render the last button by default
    buttons.back().draw = false;

    if (opts.startImmediately)
    {
        startClock();
    }

    // main update loop
    while (!close)
    {
        update();
    }
}

void Program::update()
{
    eventHandler();

    // update clock
    switch (clock.state)
    {
        case ClockState::RINGING:
            clock.loopSong();
            break;
        case ClockState::RUNNING_TIMER:
            clock.updateTimer();
            break;
        case ClockState::NOT_SET:
            break;
        case ClockState::SHOW_CLOCK:
            clock.showTime();
            break;
    }

    render();
}

void Program::eventHandler()
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        if ((e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) || e.type == SDL_QUIT)
            close = true;

        // mouse is pressed
        else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT)
        {
            // get click position
            int mouseX = e.button.x;
            int mouseY = e.button.y;

            // if any of the buttons is clicked
            for (auto& b : buttons)
            {
                if (mouseX >= b.rect.x && mouseX <= b.rect.x + b.rect.w && mouseY >= b.rect.y && mouseY <= b.rect.y + b.rect.h)
                {
                    if (b.click)
                        b.click(b);
                }
            }
        }

        // Handle key press event
        else if (e.type == SDL_KEYDOWN)
        {
            switch (e.key.keysym.sym)
            {
                case SDLK_SPACE:
                {
                    if (clock.state == ClockState::RINGING)
                    {
                        clock.state = ClockState::SHOW_CLOCK;
                        buttons.back().draw = true;
                        SDL_Log("Space key pressed, clockstate is back to normal(NOT_SET)");
                    }
                    else
                    {
                        SDL_Log("Space key pressed, while clock is not ringing");
                    }
                }
                break;
                case SDLK_RETURN:
                {
                    if (clock.state == ClockState::NOT_SET)
                    {
                        SDL_Log("Enter pressed in correct context");
                        startClock();
                    }
                    else
                    {
                        SDL_Log("Pressed Enter while clock is not in NOT_SET mode");
                    }
                }
                break;
            }
        }
    }
}

void Program::startClock()
{
    bgColor.black();
    clock.startTimer();
}

void Program::render()
{
    // render background
    Renderer::setColor(bgColor);
    Renderer::clear();

    clock.render();

    // render buttons
    for (auto& button : buttons)
        button.render();

    // main SDL rendering
    Renderer::render();
}
