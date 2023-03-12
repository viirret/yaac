#include "Config.hh"

#include <SDL2/SDL.h>

Config::Config(const std::string& fileName, char delimiter)
{
    try
    {
        std::ifstream file(fileName);

        if (!file.is_open())
        {
            throw std::runtime_error("Failed to open file");
        }

        std::stringstream buffer;

        buffer << file.rdbuf();
        std::string content = buffer.str();

        size_t start = 0;
        size_t end = content.find('\n');

        while (end != std::string::npos)
        {
            std::string line = content.substr(start, end - start);
            size_t pos = line.find(delimiter);

            if (pos != std::string::npos)
            {
                std::string key = line.substr(0, pos);
                std::string value = line.substr(pos + 1);

                configData[key] = value;
            }

            start = end + 1;
            end = content.find('\n', start);
        }
    }
    catch (const std::exception& e)
    {
        SDL_Log("Error in configuration file!");
    }
}

std::string Config::get(const std::string& key) const
{
    auto it = configData.find(key);

    if (it == configData.end())
        return "";

    return it->second;
}
