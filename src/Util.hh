#ifndef YAAC_UTIL_HH
#define YAAC_UTIL_HH

#include "Color.hh"

#include <SDL2/SDL_mixer.h>

namespace Util
{
Color readHexColor(std::string hex, const Color& defaultColor);
Mix_Music* loadMusic(const std::string& path);
} // namespace Util

#endif
