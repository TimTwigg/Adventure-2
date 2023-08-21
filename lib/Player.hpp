// Updated: 15 August 2023

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
        bool inInventory(std::string obj, unsigned int count = 1) const noexcept;
        // returns true if a container with the correct liquid is in the player's inventory
        bool inInventory(std::string container, std::string liquid) const noexcept;
        void addItem(OBJCLASS objClass, std::string obj, unsigned int count = 1);
        void addItem(std::string code);
        void addItem(Object* item);
        // access const info on an item in the inventory
        const Object* readItem(std::string item) const;
        Object* accessItem(std::string item);
        std::vector<Object*> getAll(std::string item);
        // removes the item from inventory and returns it
        // if a resource, remove multiple simultaneously
        // if not a resource, only 1 is removed
        std::shared_ptr<Object> removeItem(std::string obj, unsigned int count = 1);
        // removes the appropriate container
        std::shared_ptr<Object> removeContainer(std::string obj, std::string content = "empty");
        // uses a tool or weapon by removing a hitpoint from it
        // returns false if the tool is broken
        bool use(std::string tool);
        int itemCount(std::string obj) const noexcept;
        std::string listInventory() const noexcept;
        std::string getMe() const noexcept;
        std::string getHT() const noexcept;
        std::string getHP() const noexcept;
        
        void addWealth(unsigned int amount) noexcept;
        void removeWealth(unsigned int amount);

        // damage the player
        void damage(double dmg);
        // heal the player
        void heal(double hp) noexcept;
        // the player's attack damage
        double attackDmg(double weaponDmgModifier);
        double fistDmg();

        void addXP(int xp) noexcept;
        
        void reduceHT(double hunger, double thirst);
        void eat(double points) noexcept;
        void drink(double points) noexcept;

        double weight() const noexcept;

        void passTime(int minutes) noexcept;

        bool isDead() const noexcept;

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