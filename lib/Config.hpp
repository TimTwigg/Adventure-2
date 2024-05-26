// updated 26 May 2024

#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include <vector>

enum struct ConfigType {
    STRING,
    INT
};

struct ConfigValue {
    std::string key;
    ConfigType type;
    std::string strValue = "";
    int intValue = 0;
};

class Config {
    public:
        Config(std::string configFile = "config.ini");
        // Read config option with string value
        std::string getStringOption(std::string key) const noexcept;
        // Read config option with integer value
        int getIntOption(std::string key) const noexcept;
        // Get config value
        ConfigValue getConfigValue(std::string key) const noexcept;
    
    private:
        std::vector<ConfigValue> configValues;

        ConfigValue parseLine(std::string line);
};

#endif