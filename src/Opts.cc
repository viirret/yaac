#include "Opts.hh"

#include <SDL2/SDL.h>

#include <cstring>

Opts::Opts(int argc, char** argv)
{
    for (int i = 0; i < argc; i++)
        if (i != 0)
            arguments.push_back(argv[i]);

    for (int i = 0; i < arguments.size(); i++)
    {
        if (isArg(arguments[i], "-n", "--now"))
        {
            startImmediately = true;
        }

        else if (isArg(arguments[i], "-t", "--time"))
        {
            SDL_Log("Arg: %s", arguments[i + 1].c_str());
        }

        else if (isArg(arguments[i], "-c", "--close"))
        {
            closeImmediately = true;
        }

        else if (isArg(arguments[i], "-h", "--help"))
        {
            SDL_Log("Print help screen here");
            exit(0);
        }
    }
}

bool Opts::isArg(const std::string& realArgument, const std::string& checkAgainst) const
{
    return std::strcmp(realArgument.c_str(), checkAgainst.c_str()) == 0;
}

bool Opts::isArg(const std::string& realArgument, const std::string& checkAgainst1, const std::string& checkAgainst2) const
{
    return isArg(realArgument, checkAgainst1) || isArg(realArgument, checkAgainst2);
}
