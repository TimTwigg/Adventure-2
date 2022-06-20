// Updated: 19 June 2022

#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <vector>
#include <memory>
#include "SkillSets.hpp"
#include "Object.hpp"
#include "RandomGenerator.hpp"
#include "json.hpp"
using json = nlohmann::json;

class Player {
    public:
        static Player* load(const std::string& path);
        static std::vector<std::string> INVALID_STAT_NAMES;

    public:
        Player(SkillSets skillset, const std::string& path, float diff_ratio = 1);
        Player(const Player& other) = delete;
        void save() const;
        
        double stat(const std::string& stat_name) const;
        std::vector<std::string> getStatNames() const noexcept;
        std::vector<std::string> getInvalidStatNames() const noexcept;
        std::string getSavepath() const noexcept;
        void setSavePath(std::string newPath) noexcept;
        SkillSets getSkillset() const noexcept;
        
        // returns true if the object is in the player's inventory, with enough multiplicity
        // if it is a resource. If it is not a resource, returns true if the object is in the
        // inventory
        bool inInventory(std::string obj, unsigned int count = 1) const noexcept;
        void addItem(OBJCLASS objClass, std::string obj, unsigned int count = 1);
        void removeItem(OBJCLASS objClass, std::string obj, unsigned int count = 1);
        int itemCount(std::string obj) const noexcept;
        std::string listInventory() const noexcept;
        std::string getMe() const noexcept;
        std::string getHT() const noexcept;
        std::string getHP() const noexcept;
        
        void addWealth(unsigned int amount) noexcept;
        void removeWealth(unsigned int amount);

        void damage(double dmg);
        void heal(double hp) noexcept;
        double attackDmg(double weaponDmgModifier);
        double fistDmg();

        void addXP(int xp) noexcept;
        
        void reduceHunger(double points) noexcept;
        void reduceThirst(double points) noexcept;
        void eat(double points) noexcept;
        void drink(double points) noexcept;

        double weight() const noexcept;

    private:
        json data;
        std::vector<std::shared_ptr<Object>> inventory;
        std::vector<std::string> invalid_stat_names;
        std::string savepath;
        RandomGenerator gen;

        void level_up();
        void level_stats();
        Player();
};

#endif