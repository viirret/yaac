#ifndef YAAC_CONFIG_HH
#define YAAC_CONFIG_HH

#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>

// This class reads configuration files efficiently

class Config
{
public:
    Config(const std::string& fileName, char delimiter);
    std::string get(const std::string& key) const;

private:
    std::unordered_map<std::string, std::string> configData;
};

#endif
