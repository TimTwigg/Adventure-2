// Updated: 9 December 2021

#include <string>
#include <map>
#include "SkillSets.hpp"

std::map<SkillSets, std::string> SET::index() {
    return {
        {SkillSets::TRAVELER, "Traveller"},
        {SkillSets::WARRIOR, "Warrior"},
        {SkillSets::BRAWLER, "Brawler"},
        {SkillSets::SCOUT, "Scout"},
        {SkillSets::LUMBERJACK, "Lumberjack"},
        {SkillSets::MINER, "Miner"},
        {SkillSets::FORAGER, "Forager"},
        {SkillSets::HUNTER, "Hunter"},
        {SkillSets::JESTER, "Jester"}
    };
}

std::string SET::to_string(SkillSets skillset) {
    return index()[skillset];
}

SkillSets SET::to_skillset(std::string skillset) {
    for (auto& pair : index()) {
        if (pair.second == skillset) {
            return pair.first;
            break;
        }
    }
    return SkillSets::TRAVELER;
}