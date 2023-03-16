#include "Window.hh"

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

Window::Window(const std::string& title)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
        return;
    }

    SDL_Log("SDL Initialized");

    // get monitor size
    SDL_DisplayMode display;
    if (SDL_GetCurrentDisplayMode(0, &display))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't get display info: %s", SDL_GetError());
        return;
    }

    // create window
    window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, display.w, display.h, SDL_WINDOW_RESIZABLE);
    if (!window)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window");
        return;
    }

    // create renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (!renderer)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create renderer: %s", SDL_GetError());
        return;
    }
    else
    {
        Renderer::set(renderer);
    }

    // update window size
    SDL_GetWindowSize(window, &size.x, &size.y);
    SDL_Log("Window created!");

    // initialize other SDL stuff
    TTF_Init();

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to open audio: %s", Mix_GetError());
        return;
    }
}

Window::~Window()
{
    if (renderer)
        SDL_DestroyRenderer(renderer);
    if (window)
        SDL_DestroyWindow(window);
    Mix_CloseAudio();
    TTF_Quit();
    SDL_Quit();
}

Vec2i Window::swapSize(Vec2i newSize)
{
    Vec2i oldSize = size;
    size = newSize;
    return oldSize;
}
