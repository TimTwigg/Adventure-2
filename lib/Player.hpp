// Updated: 26 January 2022

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
        void level_up();
        double stat(const std::string& stat_name) const;
        std::vector<std::string> getStatNames() const noexcept;
        std::vector<std::string> getInvalidStatNames() const noexcept;
        std::string getSavepath() const noexcept;
        SkillSets getSkillset() const noexcept;
        
        bool inInventory(OBJCLASS objClass, std::string obj, unsigned int count = 1) const noexcept;
        void addItem(OBJCLASS objClass, std::string obj, unsigned int count = 1);
        void removeItem(OBJCLASS objClass, std::string obj, unsigned int count = 1);
        
        void addWealth(unsigned int amount) noexcept;
        void removeWealth(unsigned int amount) noexcept;

        void damage(double dmg) noexcept;
        void heal(double hp) noexcept;

        void addXP(int xp) noexcept;
        
        void reduceHunger(double points) noexcept;
        void reduceThirst(double points) noexcept;

    private:
        json data;
        std::vector<std::string> invalid_stat_names;

        void level_stats();
};

#endif