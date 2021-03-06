// Updated: 7 July 2022

#include <string>
#include <fstream>
#include <map>
#include <vector>
#include <algorithm>
#include <filesystem>
#include "Player.hpp"
#include "SkillSets.hpp"
#include "StatDefaults.hpp"
#include "AdventureException.hpp"
#include "Container.hpp"
#include "Resource.hpp"
#include "CResource.hpp"
#include "Tool.hpp"
#include "Weapon.hpp"
#include "Formulae.hpp"
#include "StringHelpers.hpp"
#include "json.hpp"
using json = nlohmann::json;
namespace fs = std::filesystem;

#include <iostream>

// names of data contents whose values are not numeric
std::vector<std::string> Player::INVALID_STAT_NAMES{"skillset", "ratios"};

Player::Player(SkillSets skillset, const std::string& path, float diff_ratio) : invalid_stat_names{Player::INVALID_STAT_NAMES} {
    if (path.size() < 1) throw AdventureException("Player: save path required");
    std::string savefile = path;
    int num{0};
    while (true) {
        fs::path p{savefile};
        if (!fs::exists(p)) break;
        ++num;
        savefile = path + "_" + std::to_string(num);
    }

    savepath = savefile;

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
    data["consumption_ratio"] = skset["consumption_ratio"].get<double>();
    data["chopping_ratio"] = skset["chopping_ratio"].get<double>();
    data["mining_ratio"] = skset["mining_ratio"].get<double>();
    data["carry_ratio"] = skset["carry_ratio"].get<double>();
    data["wealth"] = 0;
    data["ratios"] = skset;
    data["diff_ratio"] = diff_ratio;
    data["time"] = 6.0;
    data["days"] = 1;
}

Player* Player::load(const std::string& path) {
    fs::path p{path};
    if (!fs::exists(p)) throw AdventureException("Player::load save does not exist: " + path);

    Player* player = new Player();
    player->invalid_stat_names = Player::INVALID_STAT_NAMES;
    player->savepath = path;

    std::ifstream i;
    i.open(player->savepath + "\\player.game");
    json filedata;
    i >> filedata;
    i.close();
    player->data = filedata[0];

    std::for_each(filedata[1].begin(), filedata[1].end(), [&](std::string s){
        if (s.substr(0, 8) == "RESOURCE") player->inventory.push_back(std::shared_ptr<Object>(Resource::fromString(s)));
        else if (s.substr(0, 9) == "CRESOURCE") player->inventory.push_back(std::shared_ptr<Object>(CResource::fromString(s)));
        else if (s.substr(0, 9) == "CONTAINER") player->inventory.push_back(std::shared_ptr<Object>(Container::fromString(s)));
        else if (s.substr(0, 4) == "TOOL") player->inventory.push_back(std::shared_ptr<Object>(Tool::fromString(s)));
        else if (s.substr(0, 6) == "WEAPON") player->inventory.push_back(std::shared_ptr<Object>(Weapon::fromString(s)));
        else throw AdventureException("Player::Player(const std::string& savepath) (" + path + ") Unrecognized inventory string: " + s);
    });

    return player;
}

Player::Player() {}

void Player::save() const {
    std::string path_string{savepath};
    fs::path p{path_string};
    if (!fs::exists(p)) fs::create_directory(p);
    path_string += "\\player.game";
    std::ofstream o;
    o.open(path_string);

    json to_go = json::array();
    to_go.push_back(data);
    to_go.push_back(json::array());
    for (int i = 0; i < inventory.size(); ++i) {
        Object* o = inventory[i].get();
        to_go[1].push_back(o->operator std::string());
    }

    o << to_go.dump(4) << std::endl;
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
    return savepath;
}

void Player::setSavePath(std::string newPath) noexcept {
    savepath = newPath;
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
            inventory.push_back(std::shared_ptr<Object>(new Resource(obj, count)));
            break;
        case OBJCLASS::CRESOURCE:
            inventory.push_back(std::shared_ptr<Object>(new CResource(obj, count)));
            break;
        case OBJCLASS::CONTAINER:
            for (int i = 0; i < count; ++i) inventory.push_back(std::shared_ptr<Object>(new Container(obj)));
            break;
        case OBJCLASS::TOOL:
            for (int i = 0; i < count; ++i) inventory.push_back(std::shared_ptr<Object>(new Tool(obj)));
            break;
        case OBJCLASS::WEAPON:
            for (int i = 0; i < count; ++i) inventory.push_back(std::shared_ptr<Object>(new Weapon(obj)));
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

std::string Player::listInventory() const noexcept {
    if (inventory.size() == 0) return "You got nothin!";
    else {
        std::vector<std::string> i;
        std::for_each(inventory.begin(), inventory.end(), [&](const std::shared_ptr<Object>& o){i.push_back(o->printString());});
        std::string article;
        if (i[0][i[0].size()-1] == 's') article = "";
        else if (strHelp::isVowel(i[0][0])) article = "an ";
        else article = "a ";

        if (i.size() == 1) return "You have " + article + i[0] + ".";
        else {
            std::string list = i[0];
            std::for_each(i.begin()+1, i.end()-1, [&](const std::string& s){
                if (s[s.size()-1] == 's') list = list + ", " + s;
                else if (strHelp::isVowel(s[0])) list = list + ", an " + s;
                else list = list + ", a " + s;
            });
            const std::string& thing = i[i.size()-1];
            if (thing[thing.size()-1] == 's') list = list + ", and " + thing;
            else list = list + ", and a " + thing;
            return "You have " + article + list + ".";
        }
    }
}

std::string Player::getMe() const noexcept {
    std::string diff;
    int diff_r = data["diff_ratio"].get<float>();
    if (diff_r == 0.5) diff = "Easy";
    else if (diff_r == 1.0) diff = "Medium";
    else diff = "Hard";
    std::string me =
        "  Class          | " + data["skillset"].get<std::string>() + "\n" +
        "  Level          | " + std::to_string(data["level"].get<int>()) + "\n" +
        "  XP             | " + std::to_string(data["xp"].get<int>()) + "\n" +
        "  Wealth         | " + std::to_string(data["wealth"].get<int>()) + "\n" +
        "  Difficulty     | " + diff + "\n" +
        "  Health         | " + std::to_string(data["health"].get<int>()) + " / " + std::to_string(data["max_health"].get<int>()) + "\n" +
        "  Hunger         | " + std::to_string(data["hunger"].get<int>()) + " / " + std::to_string(data["max_hunger"].get<int>()) + "\n" +
        "  Thirst         | " + std::to_string(data["thirst"].get<int>()) + " / " + std::to_string(data["max_thirst"].get<int>()) + "\n" +
        "  Base Damage    | " + std::to_string(data["base_damage"].get<int>()) + "\n" +
        "  Fist Damage    | " + std::to_string(data["fist_base_damage"].get<int>()) + "\n" +
        "  Inventory      | " + listInventory();
    return me;
}

std::string Player::getHT() const noexcept {
    return "  Hunger    | " + std::to_string(data["hunger"].get<int>()) + " / " + std::to_string(data["max_hunger"].get<int>()) + "\n" +
        "  Thirst    | " + std::to_string(data["thirst"].get<int>()) + " / " + std::to_string(data["max_thirst"].get<int>());
}

std::string Player::getHP() const noexcept {
    return "  Health    | " + std::to_string(data["health"].get<int>()) + " / " + std::to_string(data["max_health"].get<int>());
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

double Player::attackDmg(double weaponDmgModifier) {
    return Formula::playerDamage(stat("base_damage") * weaponDmgModifier, gen, stat("diff_ratio"));
}

double Player::fistDmg() {
    return Formula::playerDamage(stat("fist_damage"), gen, stat("diff_ratio"));
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

void Player::passTime(int minutes) noexcept {
    double t = data["time"].get<double>() + minutes;
    if (t >= 24) {
        t -= 24;
        data["days"] = data["days"].get<int>() + 1;
    }
    data["time"] = t;
}