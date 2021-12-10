// Updated: 9 December 2021

#include <string>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <map>
#include "Player.hpp"
#include "SkillSets.hpp"
#include "StatDefaults.hpp"
#include "..\\tp_lib\\json.hpp"
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
    std::cout << savefile << std::endl;

    json skset = getSet(skillset);

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

json getSet(SkillSets skillset) {
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
            skset = sets["Jester"]; // change this
            break;
    }

    return skset;
}

void Player::level_stats() {
    SkillSets ss = SET::to_skillset(data["skillset"].get<std::string>());
    json skset = getSet(ss);
    std::map<std::string, float> bonuses;
    for (auto& pair : skset.items()) {
        if (pair.value() > 1) bonuses.emplace(pair.key(), DEFAULT::stat_level_bonus);
        else bonuses.emplace(pair.key(), DEFAULT::stat_level_no_bonus);
    }

    for (auto& pair : bonuses) {
        data[pair.first] = data[pair.first].get<float>() * pair.second;
    }
}