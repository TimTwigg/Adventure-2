// Updated: 20 January 2022

#ifndef SKILLSETS_HPP
#define SKILLSETS_HPP

#include <map>
#include "json.hpp"
using json = nlohmann::json;

enum class SkillSets {
    TRAVELER,
    WARRIOR,
    BRAWLER,
    SCOUT,
    LUMBERJACK,
    MINER,
    FORAGER,
    HUNTER,
    JESTER,
};

namespace SET {
    std::map<SkillSets, std::string> index();
    std::string to_string(SkillSets skillset);
    SkillSets to_skillset(std::string skillset);
}

#endif