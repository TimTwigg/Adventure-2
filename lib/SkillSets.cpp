// Updated: 22 January 2022

#include <map>
#include <string>
#include <fstream>
#include <vector>
#include "RandomGenerator.hpp"
#include "SkillSets.hpp"
#include "json.hpp"
using json = nlohmann::json;

json SET::getSet(SkillSets skillset) {
    std::ifstream i;
    json sets;
    i.open("data\\skillsets.json");
    i >> sets;
    i.close();
    
    json skset;
    switch (skillset) {
        case SkillSets::TRAVELER:
            skset = sets["Traveler"];
            break;
        case SkillSets::WARRIOR:
            skset = sets["Warrior"];
            break;
        case SkillSets::BRAWLER:
            skset = sets["Brawler"];
            break;
        case SkillSets::SCOUT:
            skset = sets["Scout"];
            break;
        case SkillSets::LUMBERJACK:
            skset = sets["Lumberjack"];
            break;
        case SkillSets::MINER:
            skset = sets["Miner"];
            break;
        case SkillSets::FORAGER:
            skset = sets["Forager"];
            break;
        case SkillSets::HUNTER:
            skset = sets["Hunter"];
            break;
        case SkillSets::JESTER:
            RandomGenerator gen;
            skset = json::object();
            std::vector<std::string> v = {"health", "damage", "fist_damage", "speed", "consumption_rate", "chopping_rate", "mining_ratio", "hunger_ratio", "swimming_speed"};
            std::vector<double> ratios = gen.getRandDoubleVector(5, 20, 9);
            for (int i = 0; i < 9; ++i) {
                skset[v[i]] = ratios[i];
            }
            break;
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