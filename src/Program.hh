#ifndef YAAC_PROGRAM_HH
#define YAAC_PROGRAM_HH

#include "Button.hh"
#include "Clock.hh"
#include "Color.hh"
#include "Config.hh"
#include "Window.hh"

#include <SDL2/SDL_ttf.h>

#include <vector>

class Program
{
public:
    Program(int argc, char** argv);

private:
    // cmd arguments
    int argc;
    char** argv;

    // Configuration
    Config config;

    // window object that starts SDL
    Window window;

    // font used throughout this program
    TTF_Font* mainFont;

    // backgroundcolor
    Color bgColor;

    // the clock object that get's input from buttons here
    Clock clock;

    // there are multiple buttons in our alarm clock
    std::vector<Button> buttons;

    // is our program running
    bool close = false;

    // update clock object
    void update();

    // functions called by update
    void eventHandler();
    void render();
};

#endif
