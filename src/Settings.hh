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

const std::string SONGDIR = ExpandTilde("~/.config/yaac/songs/");
const std::string ASSETDIR = ExpandTilde("~/.config/yaac/");

} // namespace Settings

#endif
