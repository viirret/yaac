#ifndef YAAC_PROGRAM_HH
#define YAAC_PROGRAM_HH

#include "Button.hh"
#include "Clock.hh"
#include "Config.hh"
#include "Opts.hh"
#include "Window.hh"

#include <SDL2/SDL_ttf.h>

#include <vector>

class Program
{
public:
    Program(int argc, char** argv);

private:
    // Cmd arguments.
    Opts opts;

    // Configuration.
    Config config;

    // Window object that starts SDL.
    Window window;

    // Font used throughout this program.
    TTF_Font* mainFont;

    // Backgroundcolor
    Color bgColor;

    // The clock object that get's input from buttons here.
    Clock clock;

    // There are multiple buttons in our alarm clock.
    std::vector<Button> buttons;

    // Is our program running.
    bool close = false;

    // Update clock object.
    void update();

    // Functions called by update.
    void eventHandler();
    void render();

    // Start measuring time.
    void startClock();

    // Check if Linux process is running.
    bool isProcessRunning(const std::string& processName);

    // Check that sound system works.
    bool playSound(const std::string& path);
};

#endif
