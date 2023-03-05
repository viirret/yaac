#ifndef YAAC_UTIL_HH
#define YAAC_UTIL_HH

#include "Color.hh"

namespace Util
{
Color readHexColor(std::string hex, const Color& defaultColor);
}

#endif
