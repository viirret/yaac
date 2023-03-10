#ifndef YAAC_COLOR_HH
#define YAAC_COLOR_HH

#include <SDL2/SDL.h>

#include <ostream>
#include <sstream>

struct Color
{
    Color()
    {
    }

    Color(int r, int g, int b, int a)
        : r(r), g(g), b(b), a(a)
    {
    }

    friend std::ostream& operator<<(std::ostream& os, const Color& color)
    {
        os << "r:" << color.r << " g:" << color.g << " b:" << color.b << " a:" << color.a;
        return os;
    }

    std::string getInfo() const
    {
        std::stringstream ss;
        ss << "r:" << r << " g:" << g << " b:" << b << " a:" << a;
        return ss.str();
    }

    SDL_Color getSDLColor() const
    {
        return {static_cast<Uint8>(r), static_cast<Uint8>(g), static_cast<Uint8>(b), static_cast<Uint8>(a)};
    }

    void black()
    {
        r = 0;
        g = 0;
        b = 0;
        a = 0;
    }

    void white()
    {
        r = 255;
        g = 255;
        b = 255;
        a = 0;
    }

    void red()
    {
        r = 255;
        b = 0;
        g = 0;
        a = 0;
    }

    void change(int r, int g, int b, int a)
    {
        this->r = r;
        this->g = g;
        this->b = b;
        this->a = a;
    }

    void change(const Color& color)
    {
        this->r = color.r;
        this->g = color.g;
        this->b = color.b;
        this->a = color.a;
    }

    int r, g, b, a;
};

#endif
