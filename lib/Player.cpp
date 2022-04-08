// Updated: 7 April 2022

#include <string>
#include <fstream>
#include <iomanip>
#include <map>
#include <vector>
#include <algorithm>
#include "Player.hpp"
#include "SkillSets.hpp"
#include "StatDefaults.hpp"
#include "AdventureException.hpp"
#include "Container.hpp"
#include "Resource.hpp"
#include "CResource.hpp"
#include "Tool.hpp"
#include "Weapon.hpp"
#include "json.hpp"
using json = nlohmann::json;

namespace {
    // names of data contents whose values are not numeric
    std::vector<std::string> INVALID_STAT_NAMES{"skillset", "savepath", "ratios"};
}

Player::Player(SkillSets skillset, const std::string& savepath) : invalid_stat_names{INVALID_STAT_NAMES} {
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

    json skset = SET::getSet(skillset);

    data["level"] = 1;
    data["xp"] = 0;
    data["skillset"] = skillset;
    data["health"] = DEFAULT::health * skset["health"].get<double>();
    data["max_health"] = DEFAULT::health * skset["health"].get<double>();
    data["base_damage"] = DEFAULT::damage * skset["damage"].get<double>();
    data["fist_base_damage"] = DEFAULT::fist_damage * skset["fist_damage"].get<double>();
    data["hunger"] = DEFAULT::hunger * skset["hunger_ratio"].get<double>();
    data["max_hunger"] = DEFAULT::hunger * skset["hunger_ratio"].get<double>();
    data["thirst"] = DEFAULT::thirst * skset["hunger_ratio"].get<double>();
    data["max_thirst"] = DEFAULT::thirst * skset["hunger_ratio"].get<double>();
    data["carry_weight"] = DEFAULT::carry_weight * skset["carry_ratio"].get<double>();
    data["speed"] = skset["speed"].get<double>();
    data["swimming_speed"] = skset["swimming_speed"].get<double>();
    data["consumption_ratio"] = skset["consumption_ratio"].get<double>();
    data["chopping_ratio"] = skset["chopping_ratio"].get<double>();
    data["mining_ratio"] = skset["mining_ratio"].get<double>();
    data["carry_ratio"] = skset["carry_ratio"].get<double>();
    data["wealth"] = 0;
    data["savepath"] = savefile;
    data["ratios"] = skset;
}

Player::Player(const std::string& savepath) : invalid_stat_names{INVALID_STAT_NAMES} {
    std::ifstream i;
    i.open(savepath);
    if (!i) throw AdventureException("Player::Player() save file does not exist.");
    json filedata;
    i >> filedata;
    i.close();
    data = filedata[0];
    std::for_each(filedata[1].begin(), filedata[1].end(), [&](std::string s){
        if (s.substr(0, 8) == "RESOURCE") inventory.push_back(std::unique_ptr<Object>(Resource::from_string(s)));
        else if (s.substr(0, 9) == "CRESOURCE") inventory.push_back(std::unique_ptr<Object>(CResource::from_string(s)));
        else if (s.substr(0, 9) == "CONTAINER") inventory.push_back(std::unique_ptr<Object>(Container::from_string(s)));
        else if (s.substr(0, 4) == "TOOL") inventory.push_back(std::unique_ptr<Object>(Tool::from_string(s)));
        else if (s.substr(0, 6) == "WEAPON") inventory.push_back(std::unique_ptr<Object>(Weapon::from_string(s)));
        else throw AdventureException("Player::Player(const std::string& savepath) (" + savepath + ") Unrecognized inventory string: " + s);
    });
}

void Player::save() const {
    std::ofstream o;
    o.open(data["savepath"].get<std::string>());

    json to_go = json::array();
    to_go.push_back(data);
    to_go.push_back(json::array());
    for (int i = 0; i < inventory.size(); ++i) {
        Object* o = inventory[i].get();
        to_go[1].push_back(o->operator std::string());
    }

    o << std::setw(4) << to_go << std::endl;
    o.close();
}

void Player::level_up() {
    if (data["xp"].get<int>() < DEFAULT::xp_per_level) return;

    int excess_xp = data["xp"].get<int>() - DEFAULT::xp_per_level;
    data["level"] = data["level"].get<unsigned int>() + 1;
    data["xp"] = excess_xp;

    // get level-based ratio for stat upgrades
    double ratio;
    unsigned int level = data["level"].get<unsigned int>();
    if (level < DEFAULT::level_1) ratio = DEFAULT::level_ratio_1;
    else if (level < DEFAULT::level_2) ratio = DEFAULT::level_ratio_2;
    else if (level < DEFAULT::level_3) ratio = DEFAULT::level_ratio_3;
    else ratio = DEFAULT::level_ratio_uncapped;

    // upgrade stats
    data["health"] = static_cast<unsigned int>(data["health"].get<unsigned int>() * ratio);
    data["base_damage"] = static_cast<unsigned int>(data["base_damage"].get<unsigned int>() * ratio);
    data["fist_base_damage"] = static_cast<unsigned int>(data["fist_base_damage"].get<unsigned int>() * ratio);

    // level all stats if appropriate
    if (level % DEFAULT::stat_level_interval == 0) level_stats();
}

void Player::level_stats() {
    json skset = SET::getSet(data["skillset"].get<SkillSets>());
    std::map<std::string, double> bonuses;
    for (auto& pair : skset.items()) {
        if (pair.value() > 1) bonuses.emplace(pair.key(), DEFAULT::stat_level_bonus);
        else bonuses.emplace(pair.key(), DEFAULT::stat_level_no_bonus);
    }

    for (auto& pair : bonuses) {
        data[pair.first] = data[pair.first].get<double>() * pair.second;
    }
}

double Player::stat(const std::string& stat_name) const {
    if (std::find(invalid_stat_names.begin(), invalid_stat_names.end(), stat_name) != invalid_stat_names.end()) throw AdventureException("Non-Numeric Stat Name: " + stat_name + " :: use specific method to access");
    else if (data.count(stat_name) == 0) throw AdventureException("Invalid Stat Name: " + stat_name);
    return data[stat_name];
}

std::vector<std::string> Player::getStatNames() const noexcept {
    std::vector<std::string> v;
    for (const auto& i : data.items()) v.push_back(i.key());
    return v;
}

std::vector<std::string> Player::getInvalidStatNames() const noexcept {
    return invalid_stat_names;
}

std::string Player::getSavepath() const noexcept {
    return data["savepath"].get<std::string>();
}

SkillSets Player::getSkillset() const noexcept {
    return data["skillset"].get<SkillSets>();
}

bool Player::inInventory(std::string obj, unsigned int count) const noexcept {
    if (count == 0) return true;
    for (auto it = inventory.begin(); it != inventory.end(); ++it) {
        Object* o = it->get();
        if (o->getName() == obj) {
            if (o->getType() == OBJCLASS::RESOURCE) {
                Resource* r = static_cast<Resource*>(o);
                return r->getCount() >= count;
            }
            return true;
        }
    }
    return false;
}

void Player::addItem(OBJCLASS objClass, std::string obj, unsigned int count) {
    if (objClass == OBJCLASS::RESOURCE) {
        for (auto it = inventory.begin(); it != inventory.end(); ++it) {
            Object* o = it->get();
            if (o->getName() == obj) {
                Resource* r = static_cast<Resource*>(o);
                r->add(count);
                return;
            }
        }
    }

    switch (objClass) {
        case OBJCLASS::RESOURCE:
            inventory.push_back(std::unique_ptr<Object>(new Resource(obj, count)));
            break;
        case OBJCLASS::CRESOURCE:
            inventory.push_back(std::unique_ptr<Object>(new CResource(obj, count)));
            break;
        case OBJCLASS::CONTAINER:
            for (int i = 0; i < count; ++i) inventory.push_back(std::unique_ptr<Object>(new Container(obj)));
            break;
        case OBJCLASS::TOOL:
            for (int i = 0; i < count; ++i) inventory.push_back(std::unique_ptr<Object>(new Tool(obj)));
            break;
        case OBJCLASS::WEAPON:
            for (int i = 0; i < count; ++i) inventory.push_back(std::unique_ptr<Object>(new Weapon(obj)));
            break;
        default:
            throw AdventureException("Player::addItem invalid OBJCLASS");
            break;
    }
}

void Player::removeItem(OBJCLASS objClass, std::string obj, unsigned int count) {
    for (auto it = inventory.begin(); it != inventory.end(); ++it) {
        Object* o = it->get();
        if (o->getName() == obj) {
            if (o->getType() == OBJCLASS::RESOURCE) {
                Resource* r = static_cast<Resource*>(o);
                if (r->getCount() > count) r->remove(count);
                else if (r->getCount() == count) inventory.erase(it);
                else throw AdventureException("Player::removeItem not enough to remove: " + obj + " " + std::to_string(count));
            }
            else inventory.erase(it);
            return;
        }
    }
    throw AdventureException("Player::removeItem could not find item: " + obj);
}

int Player::itemCount(std::string obj) const noexcept {
    int count = 0;
    for (auto it = inventory.begin(); it != inventory.end(); ++it) {
        Object* o = it->get();
        if (o->getName() == obj) {
            if (o->getType() == OBJCLASS::RESOURCE || o->getType() == OBJCLASS::CRESOURCE) {
                Resource* r = static_cast<Resource*>(o);
                count += r->getCount();
            }
            else ++count;
        }
    }
    return count;
}

void Player::addWealth(unsigned int amount) noexcept {
    data["wealth"] = data["wealth"].get<int>() + amount;
}

void Player::removeWealth(unsigned int amount) {
    if (data["wealth"].get<int>() < amount) throw AdventureException("Insufficient Wealth: Cannot remove " + 
        std::to_string(amount) + " from " + std::to_string(data["wealth"].get<int>()));
    data["wealth"] = data["wealth"].get<int>() - amount;
}

void Player::damage(double dmg) {
    data["health"] = data["health"].get<double>() - dmg;
    if (data["health"].get<int>() < 1) throw AdventureException("Oh No! You Died.");
}

void Player::heal(double hp) noexcept {
    data["health"] = data["health"].get<double>() + hp;
    if (data["health"].get<double>() > data["max_health"].get<double>()) data["health"] = data["max_health"];
}

void Player::addXP(int xp) noexcept {
    data["xp"] = data["xp"].get<int>() + xp;
    level_up();
}

void Player::reduceHunger(double points) noexcept {
    data["hunger"] = data["hunger"].get<double>() - points;
    if (data["hunger"].get<double>() < 0) data["hunger"] = 0;
}

void Player::reduceThirst(double points) noexcept {
    data["thirst"] = data["thirst"].get<double>() - points;
    if (data["thirst"].get<double>() < 0) data["thirst"] = 0;
}

void Player::eat(double points) noexcept {
    data["hunger"] = data["hunger"].get<double>() + points;
    if (data["hunger"].get<double>() > data["max_hunger"].get<double>()) data["hunger"] = data["max_hunger"];
}

void Player::drink(double points) noexcept {
    data["thirst"] = data["thirst"].get<double>() + points;
    if (data["thirst"].get<double>() > data["max_thirst"].get<double>()) data["thirst"] = data["max_thirst"];
}

double Player::weight() const noexcept {
    double carryable = 0.0;
    int n = inventory.size();
    for (int i = 0; i < n; ++i) {
        Object* o = inventory[i].get();
        carryable += o->getWeight();
    }
    return carryable;
}