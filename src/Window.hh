#include "Renderer.hh"
#include "Vector2.hh"

#include <SDL2/SDL.h>

#include <string>

class Window
{
public:
    Window(const std::string& title);
    ~Window();

    bool valid()
    {
        return window;
    }

    Vec2i swapSize(Vec2i newSize);
    Vec2i getSize()
    {
        return size;
    }

private:
    SDL_Window* window;
    Vec2i size;

    SDL_Renderer* renderer;
};
