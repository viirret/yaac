#ifndef YAAC_OPTS_HH
#define YAAC_OPTS_HH

#include <string>
#include <vector>

class Opts
{
public:
    // Constructor.
    Opts(int argc, char** argv);
    bool startImmediately = false;
    bool closeImmediately = false;
    std::string wakeupTime = "";

private:
    // Storage for arguments.
    std::vector<std::string> arguments;

    // abstraction for std::strcmp
    bool isArg(const std::string& realArgument, const std::string& checkAgainst) const;
    bool isArg(const std::string&, const std::string& checkAgainst1, const std::string& checkAgainst2) const;
};

#endif
