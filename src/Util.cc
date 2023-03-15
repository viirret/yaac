#include "Util.hh"
#include "Settings.hh"
#include <SDL2/SDL_mixer.h>

namespace Util
{

Color readHexColor(std::string hex, const Color& defaultColor)
{
    // check if the input string starts with a '#' character
    if (hex.size() > 0 && hex[0] == '#')
    {
        hex.erase(0, 1);
    }

    int red, green, blue;

    try
    {
        // parse hex
        std::stringstream(hex.substr(0, 2)) >> std::hex >> red;
        std::stringstream(hex.substr(2, 2)) >> std::hex >> green;
        std::stringstream(hex.substr(4, 2)) >> std::hex >> blue;

        // clamp the RGB values to the range [0, 255]
        red = static_cast<int>(red * 255.0f / 0xFF + 0.5f);
        green = static_cast<int>(green * 255.0f / 0xFF + 0.5f);
        blue = static_cast<int>(blue * 255.0f / 0xFF + 0.5f);

        return Color(red, green, blue, 255);
    }
    catch (...)
    {
        return defaultColor;
    }
}

Mix_Music* loadMusic(const std::string& path)
{
    return Mix_LoadMUS((Settings::SONGDIR + path).c_str());
}

} // namespace Util
