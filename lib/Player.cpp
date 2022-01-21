// Updated: 20 January 2022

#include <string>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <map>
#include <vector>
#include <algorithm>
#include "Player.hpp"
#include "SkillSets.hpp"
#include "StatDefaults.hpp"
#include "json.hpp"
using json = nlohmann::json;

Player::Player(SkillSets skillset, std::string savepath) {
    std::string savefile = "saves\\" + savepath + ".game";
    int num{0};
    while (true) {
        std::ifstream i;
        i.open(savefile);
        if (!i) {
            i.close();
            break;
        }
        i.close();
        ++num;
        savefile = "saves\\" + savepath + "-" + std::to_string(num) + ".game";
    }

    unsigned int seed = time(NULL);
    json skset = Player::getSet(skillset, seed);

    data["level"] = 1;
    data["xp"] = 0;
    data["skillset"] = skillset;
    data["health"] = DEFAULT::health * skset["health"].get<float>();
    data["max_health"] = DEFAULT::health * skset["health"].get<float>();
    data["base_damage"] = DEFAULT::damage * skset["damage"].get<float>();
    data["fist_base_damage"] = DEFAULT::fist_damage * skset["fist_damage"].get<float>();
    data["hunger"] = DEFAULT::hunger * skset["hunger_ratio"].get<float>();
    data["max_hunger"] = DEFAULT::hunger * skset["hunger_ratio"].get<float>();
    data["thirst"] = DEFAULT::thirst * skset["hunger_ratio"].get<float>();
    data["max_thirst"] = DEFAULT::thirst * skset["hunger_ratio"].get<float>();
    data["speed"] = skset["speed"].get<float>();
    data["swimming_speed"] = skset["swimming_speed"].get<float>();
    data["consumption_ratio"] = skset["consumption_ratio"].get<float>();
    data["chopping_ratio"] = skset["chopping_ratio"].get<float>();
    data["mining_ratio"] = skset["mining_ratio"].get<float>();
    data["wealth"] = 0;
    data["inventory"] = json::object();
    data["savepath"] = savefile;
    data["seed"] = seed;
}

void Player::save() const {
    std::ofstream o;
    o.open(data["savepath"].get<std::string>());
    o << std::setw(4) << data << std::endl;
    o.close();
}

void Player::level_up() {
    data["level"] = data["level"].get<unsigned int>() + 1;
    data["xp"] = 0;

    float ratio;
    unsigned int level = data["level"].get<unsigned int>();
    if (level < DEFAULT::level_1) ratio = DEFAULT::level_ratio_1;
    else if (level < DEFAULT::level_2) ratio = DEFAULT::level_ratio_2;
    else if (level < DEFAULT::level_3) ratio = DEFAULT::level_ratio_3;
    else ratio = DEFAULT::level_ratio_uncapped;

    data["health"] = static_cast<unsigned int>(data["health"].get<unsigned int>() * ratio);
    data["base_damage"] = static_cast<unsigned int>(data["base_damage"].get<unsigned int>() * ratio);
    data["fist_base_damage"] = static_cast<unsigned int>(data["fist_base_damage"].get<unsigned int>() * ratio);

    if (level % DEFAULT::stat_level_interval == 0) level_stats();
}

json Player::getSet(SkillSets skillset, unsigned int seed) {
    std::ifstream i;
    json sets;
    i.open("data\\skillsets.json");
    i >> sets;
    i.close();
    
    json skset;
    switch (skillset) {
        case SkillSets::TRAVELER:
            skset = sets["Traveller"];
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
            srand(seed);
            skset = json::object();
            std::vector<std::string> v = {"health", "damage", "fist_damage", "speed", "consumption_rate", "chopping_rate", "mining_ratio", "hunger_ratio", "swimming_speed"};
            std::vector<int> selection;
            for (int i = 0; i < 6; ++i) {
                int n = rand() % 9;
                if (std::find(selection.begin(), selection.end(), n) == selection.end()) {
                    selection.push_back(n);
                }
                else --i;
            }
            for (int i = 0; i < 9; ++i) {
                if (std::find(selection.begin(), selection.end(), i) == selection.end()) {
                    skset[v[i]] = 1;
                }
                else {
                    if (rand() % 2) skset[v[i]] = 2;
                    else skset[v[i]] = 0.5;
                }
            }
            break;
    }

    return skset;
}

void Player::level_stats() {
    SkillSets ss = SET::to_skillset(data["skillset"].get<std::string>());
    json skset = Player::getSet(ss, data["seed"].get<unsigned int>());
    std::map<std::string, float> bonuses;
    for (auto& pair : skset.items()) {
        if (pair.value() > 1) bonuses.emplace(pair.key(), DEFAULT::stat_level_bonus);
        else bonuses.emplace(pair.key(), DEFAULT::stat_level_no_bonus);
    }

    for (auto& pair : bonuses) {
        data[pair.first] = data[pair.first].get<float>() * pair.second;
    }
}