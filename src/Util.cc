#include "Util.hh"
#include "Settings.hh"

#include <memory>


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

bool isProcessRunning(const std::string& processName)
{
    std::string command = "pidof " + processName;
    std::string result;
    std::shared_ptr<FILE> pipe(popen(command.c_str(), "r"), pclose);

    if (!pipe)
    {
        throw std::runtime_error("popen() failed");
    }

    char buffer[128];
    while (fgets(buffer, sizeof(buffer), pipe.get()) != nullptr)
    {
        result += buffer;
    }

    return !result.empty();
}

Mix_Music* loadSoundFromConfig(const std::string& path)
{
    return loadSound((Settings::SONGDIR + path).c_str());
}

Mix_Music* loadSound(const std::string& path)
{
    Mix_Music* sound = Mix_LoadMUS(path.c_str());

    if(sound)
    {
        return sound;
    }
    else
    {
        if (isProcessRunning("systemd"))
        {
            if (isProcessRunning("pulseaudio"))
            {
                int restart = std::system("systemctl --user restart pulseaudio.service");

                if (restart == 0)
                {
                    SDL_Log("pulseaudio restarted successfully");
                }
                else
                {
                    SDL_Log("Something wrong with restarting pulseaudio");
                }

                sound = Mix_LoadMUS(path.c_str());

            }
            else if(isProcessRunning("pipewire"))
            {
                int restart = std::system("systemctl --user restart pipewire.service");

                if (restart == 0)
                {
                    SDL_Log("pipewire restarted successfully");
                }
                else
                {
                    SDL_Log("Something wrong with restarting pipewire");
                }

                sound = Mix_LoadMUS(path.c_str());
            }
        }
    }
    return sound;

}

} // namespace Util
