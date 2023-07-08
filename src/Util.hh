#ifndef YAAC_UTIL_HH
#define YAAC_UTIL_HH

#include "Color.hh"

#include <SDL2/SDL_mixer.h>

namespace Util
{

// Utility functions.

Color readHexColor(std::string hex, const Color& defaultColor);
Mix_Music* loadMusic(const std::string& path);

} // namespace Util

#endif
