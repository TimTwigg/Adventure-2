// updated 14 June 2022

#ifndef GAMEDATA_HPP
#define GAMEDATA_HPP

#include <string>
#include "SkillSets.hpp"
#include "json.hpp"
using json = nlohmann::json;

struct GameData {
    std::string filename;
    bool isNew;
    SkillSets skillset;
    json configs;
};

#endif