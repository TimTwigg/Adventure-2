// updated 28 July 2023

#ifndef CIVILIZATION_HPP
#define CIVILIZATION_HPP

#include <string>
#include <vector>
#include <utility>
#include <map>
#include "Thing.hpp"
#include "RandomGenerator.hpp"
#include "json.hpp"
using json = nlohmann::json;
using trades = std::map<std::pair<std::string, std::string>, std::pair<unsigned int, unsigned int>>;

class Civilization : public Thing {
    public:
        static Civilization* load(json data);

    public:
        Civilization(std::string name, bool isNew = true);
        operator std::string() const noexcept override;
        json save() const noexcept;
        std::string printString() const noexcept override;
        trades getSellingTrades() const noexcept;
        trades getBuyingTrades() const noexcept;
        // buy item from player
        void buy(std::string type, const std::string& name, int quantity = 1);
        // sell item to player
        void sell(std::string type, const std::string& name, int quantity = 1);
        std::map<std::pair<std::string, std::string>, int> getLoot();
        unsigned int getXP() const noexcept;
        unsigned int getWealth() const noexcept;
        unsigned int attack() noexcept;
        bool raid(double dmg);
        bool isLooted() const noexcept;
        bool isRaided() const noexcept;

    private:
        // items sold by civ, value is (quantity, price)
        trades selling;
        // items bought by civ, value is (quantity, price)
        trades buying;
        // drops if civ is raided
        std::map<std::pair<std::string, std::string>, int> loot;
        // xp gained if civ is raided
        unsigned int xp;
        // damage dealt by civ in battle
        unsigned int damage;
        // defense power of the civ
        float defense;
        // switch indicating whether the civ has been looted
        bool looted;
        // switch for whether the civ has been entirely raided
        bool raided;
        RandomGenerator gen;

        // set trades as part of constructor
        void setTrades(json data);
};

#endif