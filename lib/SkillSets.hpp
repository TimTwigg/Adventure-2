// Updated: 14 February 2022

#ifndef SKILLSETS_HPP
#define SKILLSETS_HPP

#include <map>
#include <string>
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

NLOHMANN_JSON_SERIALIZE_ENUM(SkillSets, {
    {SkillSets::TRAVELER, "TRAVELER"},
    {SkillSets::WARRIOR, "WARRIOR"},
    {SkillSets::BRAWLER, "BRAWLER"},
    {SkillSets::SCOUT, "SCOUT"},
    {SkillSets::LUMBERJACK, "LUMBERJACK"},
    {SkillSets::MINER, "MINER"},
    {SkillSets::FORAGER, "FORAGER"},
    {SkillSets::HUNTER, "HUNTER"},
    {SkillSets::JESTER, "JESTER"},
})

namespace SET {
    json getSet(SkillSets skillset);
    std::map<SkillSets, std::string> index();
    std::string to_string(SkillSets skillset);
    SkillSets to_skillset(std::string skillset);
}

#endif