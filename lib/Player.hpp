// Updated: 11 June 2022

#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <vector>
#include <memory>
#include "SkillSets.hpp"
#include "Object.hpp"
#include "json.hpp"
using json = nlohmann::json;

class Player {
    public:
        Player(SkillSets skillset, const std::string& savepath);
        explicit Player(const std::string& savepath);
        Player(const Player& other) = delete;
        void save() const;
        
        double stat(const std::string& stat_name) const;
        std::vector<std::string> getStatNames() const noexcept;
        std::vector<std::string> getInvalidStatNames() const noexcept;
        std::string getSavepath() const noexcept;
        SkillSets getSkillset() const noexcept;
        
        // returns true if the object is in the player's inventory, with enough multiplicity
        // if it is a resource. If it is not a resource, returns true if the object is in the
        // inventory
        bool inInventory(std::string obj, unsigned int count = 1) const noexcept;
        void addItem(OBJCLASS objClass, std::string obj, unsigned int count = 1);
        void removeItem(OBJCLASS objClass, std::string obj, unsigned int count = 1);
        int itemCount(std::string obj) const noexcept;
        
        void addWealth(unsigned int amount) noexcept;
        void removeWealth(unsigned int amount);

        void damage(double dmg);
        void heal(double hp) noexcept;
        double attackDmg() const;

        void addXP(int xp) noexcept;
        
        void reduceHunger(double points) noexcept;
        void reduceThirst(double points) noexcept;
        void eat(double points) noexcept;
        void drink(double points) noexcept;

        double weight() const noexcept;

    private:
        json data;
        std::vector<std::unique_ptr<Object>> inventory;
        std::vector<std::string> invalid_stat_names;

        void level_up();
        void level_stats();
};

#endif