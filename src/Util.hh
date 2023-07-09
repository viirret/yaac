#ifndef YAAC_UTIL_HH
#define YAAC_UTIL_HH

#include "Color.hh"

#include <SDL2/SDL_mixer.h>

namespace Util
{

// Utility functions.

// Get Color from hex code.
Color readHexColor(std::string hex, const Color& defaultColor);

// Load SDL_mixer sound objects.
Mix_Music* loadSoundFromConfig(const std::string& path);
Mix_Music* loadSound(const std::string& path);

// Check if Linux process is running.
bool isProgramRunning(const std::string& processName);

} // namespace Util

#endif
