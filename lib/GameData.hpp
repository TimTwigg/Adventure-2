// updated 12 June 2022

#ifndef GAMEDATA_HPP
#define GAMEDATA_HPP

#include <string>
#include <map>
#include "SkillSets.hpp"

struct GameData {
    std::string filename;
    bool isNew;
    SkillSets skillset;
    std::map<std::string, std::string> configs;
};

#endif