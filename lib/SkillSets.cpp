// Updated: 15 February 2022

#include <map>
#include <string>
#include <vector>
#include "RandomGenerator.hpp"
#include "SkillSets.hpp"
#include "FileReader.hpp"
#include "json.hpp"
using json = nlohmann::json;

json SET::getSet(SkillSets skillset) {    
    json skset;
    if (skillset == SkillSets::JESTER) {
        RandomGenerator gen;
        skset = json::object();
        std::vector<std::string> v = {"health", "damage", "fist_damage", "speed", "consumption_ratio", "chopping_ratio", "mining_ratio", "hunger_ratio", "swimming_speed"};
        std::vector<double> ratios = gen.getRandDoubleVector(5, 20, 9);
        for (int i = 0; i < 9; ++i) {
            skset[v[i]] = ratios[i];
        }
    }
    else {
        skset = FileReader::getFromFile("skillsets.json", to_string(skillset));
    }
    return skset;
}

std::map<SkillSets, std::string> SET::index() {
    return {
        {SkillSets::TRAVELER, "Traveler"},
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