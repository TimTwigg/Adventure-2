// updated 7 June 2024

#include <string>
#include <map>
#include <utility>
#include "Civilization.hpp"
#include "AdventureException.hpp"
#include "Formulae.hpp"
#include "StringHelpers.hpp"
#include "FileReader.hpp"
#include "json.hpp"
using json = nlohmann::json;

Civilization::Civilization(std::string name, bool isNew) {
    strHelp::format(name);
    if (name.size() < 1) throw AdventureException("Civilization: name required");
    json data = FileReader::getFromFile("civ-types.json", name);
    this->name = name;
    this->value = data["loot"].get<unsigned int>();
    xp = data["xp"].get<unsigned int>();
    damage = data["counter-attack"].get<unsigned int>();
    if (isNew) {
        defense = data["defense"].get<float>();
        looted = false;
        raided = false;
        setTrades(data);
    }
}

void Civilization::setTrades(json data) {
    std::vector<std::string> buying_categories = data["buying"].get<std::vector<std::string>>();
    std::vector<std::string> selling_categories = data["selling"].get<std::vector<std::string>>();
    std::vector<std::string> loots = data["items"].get<std::vector<std::string>>();
    int count = data["count"].get<int>();

    if (count > 0 && (buying_categories.size() > 0 || selling_categories.size() > 0 || loots.size() > 0)) {
        std::function<std::map<std::string, int>(json)> getKeyValues = [](json j){
            std::map<std::string, int> m;
            for (const auto& [k, v] : j.items()) m[k] = v["value"].get<int>();
            return m;
        };
        std::function<std::vector<std::string>(json)> getKeys = [](json j){
            std::vector<std::string> v;
            for (const auto& [k, _] : j.items()) v.push_back(k);
            return v;
        };

        std::map<std::string, std::map<std::string, int>> databank;
        std::map<std::string, std::vector<std::string>> keybank;

        // load data
        json resources = FileReader::readFile("resources.json");
        databank["resources"] = getKeyValues(resources);
        keybank["resources"] = getKeys(resources);
        json tools = FileReader::readFile("tools.json");
        databank["tools"] = getKeyValues(tools);
        keybank["tools"] = getKeys(tools);
        json weapons = FileReader::readFile("weapons.json");
        databank["weapons"] = getKeyValues(weapons);
        keybank["weapons"] = getKeys(weapons);
        json containers = FileReader::readFile("containers.json");
        databank["containers"] = getKeyValues(containers);
        keybank["containers"] = getKeys(containers);
        json cresources = FileReader::readFile("craftableResources.json");
        databank["cresources"] = getKeyValues(cresources);
        keybank["cresources"] = getKeys(cresources);
        json animals = FileReader::readFile("animals.json");
        databank["animals"] = getKeyValues(animals);
        keybank["animals"] = getKeys(animals);
        json machines = FileReader::readFile("machines.json");
        databank["machines"] = getKeyValues(machines);
        keybank["machines"] = getKeys(machines);

        json liquids = FileReader::readFile("liquids.json");
        std::map<std::string, int> bankA = getKeyValues(liquids);
        std::map<std::string, int> bankB;
        std::for_each(bankA.begin(), bankA.end(), [&bankB](const std::pair<std::string, int>& p) { bankB["vial of " + p.first] = p.second; });
        databank["liquids"] = bankB;
        std::vector<std::string> liquidKeys = getKeys(liquids);
        std::remove(liquidKeys.begin(), liquidKeys.end(), "empty");
        std::for_each(liquidKeys.begin(), liquidKeys.end(), [](std::string& s) { s = "vial of " + s; });
        keybank["liquids"] = liquidKeys;
        
        for (int i = 0; i < count; ++i) {
            // buying
            int trades = buying_categories.size();
            if (trades > 0) {
                std::string category = buying_categories[gen.getRandInt(0, trades-1)];
                std::map<std::string, int> bank = databank[category];
                std::vector<std::string> keys = keybank[category];
                std::string name = keys[gen.getRandInt(0, bank.size()-1)];
                for (int j = 0; j < 10; ++j) {
                    if (buying.find(std::make_pair(category, name)) == buying.end()) break;
                    category = buying_categories[gen.getRandInt(0, trades-1)];
                    bank = databank[category];
                    keybank[category];
                    name = keys[gen.getRandInt(0, keys.size()-1)];
                }
                int price = bank[name] * (gen.getRandInt(6, 15) / 10.0);
                if (price < 1) price = 1;
                buying[std::make_pair(category, name)] = std::make_pair(gen.getRandInt(1, 10), price);
            }

            // selling
            trades = selling_categories.size();
            if (trades > 0) {
                std::string category = selling_categories[gen.getRandInt(0, trades-1)];
                std::map<std::string, int> bank = databank[category];
                std::vector<std::string> keys = keybank[category];
                std::string name = keys[gen.getRandInt(0, keys.size()-1)];
                for (int j = 0; j < 10; ++j) {
                    if (selling.find(std::make_pair(category, name)) == selling.end()) break;
                    category = selling_categories[gen.getRandInt(0, trades-1)];
                    bank = databank[category];
                    keys = keybank[category];
                    name = keys[gen.getRandInt(0, keys.size()-1)];
                }
                int price = bank[name] * (gen.getRandInt(7, 22) / 10.0);
                if (price < 1) price = 1;
                selling[std::make_pair(category, name)] = std::make_pair(gen.getRandInt(1, 10), price);
            }

            // loot items
            trades = loots.size();
            if (trades > 0) {
                std::string category = loots[gen.getRandInt(0, trades-1)];
                if (category == "wealth") {
                    loot[std::make_pair(category, category)] = gen.getRandInt(1, 6) * value;
                }
                else {
                    std::vector<std::string> keys = keybank[category];
                    std::string name = keys[gen.getRandInt(0, keys.size()-1)];
                    for (int j = 0; j < 10; ++j) {
                        if (loot.find(std::make_pair(category, name)) == loot.end()) break;
                        category = loots[gen.getRandInt(0, trades-1)];
                        keys = keybank[category];
                        name = keys[gen.getRandInt(0, keys.size()-1)];
                    }
                    int num = gen.getRandInt(0, count);
                    if (num > 0 && gen.getRandBool()) loot[std::make_pair(category, name)] = num;
                }
            }
        }   
    }
}

Civilization::operator std::string() const noexcept {
    return "CIVILIZATION, " + name;
}

json Civilization::save() const noexcept {
    json j;
    j["name"] = name;
    j["buying"] = buying;
    j["selling"] = selling;
    j["defense"] = defense;
    j["raided"] = raided;
    j["looted"] = looted;
    j["loot"] = loot;
    return j;
}

Civilization* Civilization::load(json data) {
    Civilization* c = new Civilization(data["name"].get<std::string>(), false);
    c->defense = data["defense"].get<float>();
    c->looted = data["looted"].get<bool>();
    c->raided = data["raided"].get<bool>();
    c->buying = data["buying"].get<std::map<std::pair<std::string, std::string>, std::pair<unsigned int, unsigned int>>>();
    c->selling = data["selling"].get<std::map<std::pair<std::string, std::string>, std::pair<unsigned int, unsigned int>>>();
    c->loot = data["loot"].get<std::map<std::pair<std::string, std::string>, int>>();
    return c;
}

std::string Civilization::printString() const noexcept {
    if (looted) return "looted " + name;
    else if (raided) return "raided " + name;
    else return name;
}

std::map<std::pair<std::string, std::string>, std::pair<unsigned int, unsigned int>> Civilization::getSellingTrades() const noexcept {
    return selling;
}

std::map<std::pair<std::string, std::string>, std::pair<unsigned int, unsigned int>> Civilization::getBuyingTrades() const noexcept {
    return buying;
}

void Civilization::sell(std::string type, const std::string& name, int quantity) {
    std::pair<std::string, std::string> key = std::make_pair(type, name);
    if (selling.find(key) == selling.end()) throw AdventureException("Civilization::sell not selling (" + type + ", " + name + ")");
    auto value = selling[key];
    value.first = value.first - quantity;
    if (value.first < 1) selling.erase(key);
    else selling[key] = value;
}

void Civilization::buy(std::string type, const std::string& name, int quantity) {
    std::pair<std::string, std::string> key = std::make_pair(type, name);
    if (buying.find(key) == buying.end()) throw AdventureException("Civilization::buy not buying (" + type + ", " + name + ")");
    auto value = buying[key];
    value.first = value.first - quantity;
    if (value.first < 1) buying.erase(key);
    else buying[key] = value;
}

std::map<std::pair<std::string, std::string>, int> Civilization::getLoot() {
    if (!looted) {
        looted = true;
        return loot;
    }
    else if (raided) {
        loot.clear();
        return loot;
    }
    else throw AdventureException("Must raid " + name + " before looting");
}

unsigned int Civilization::getXP() const noexcept {
    if (raided) return xp;
    else return 0;
}

unsigned int Civilization::getWealth() const noexcept {
    if (raided) return value;
    else return 0;
}

double Civilization::attack() noexcept {
    if (damage == 0) return 0;
    return Formula::civilizationDamage(damage, gen);
}

bool Civilization::raid(double dmg) {
    defense -= dmg;
    if (defense <= 0) {
        defense = 0;
        raided = true;
        return true;
    }
    return false;
}

bool Civilization::isLooted() const noexcept {
    return looted;
}

bool Civilization::isRaided() const noexcept {
    return raided;
}
