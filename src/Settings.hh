#ifndef YAAC_SETTINGS_HH
#define YAAC_SETTINGS_HH

#include <cstdlib>
#include <string>

namespace Settings
{

inline std::string ExpandTilde(const std::string& path)
{
    const char* home_dir = std::getenv("HOME");
    if (!home_dir)
        return path;

    return std::string(home_dir) + path.substr(1);
}

const std::string CONFIG = ExpandTilde("~/.config/yaac/config");
const std::string ALSASONGDIR = "/usr/share/sounds/alsa/";
const std::string SONGDIR = "/usr/share/sounds/yaac/";
const std::string FONTDIR = "/usr/share/fonts/gnu-free/FreeSans.otf";

} // namespace Settings

#endif
