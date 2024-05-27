// updated 26 May 2024

#ifndef ENEMY_HPP
#define ENEMY_HPP

#include <string>
#include <vector>
#include <memory>
#include "Entity.hpp"

enum class DangerLevel {
    PEACEFUL,
    NORMAL,
    HOSTILE
};

NLOHMANN_JSON_SERIALIZE_ENUM(DangerLevel, {
    { DangerLevel::PEACEFUL, "Peaceful" },
    { DangerLevel::NORMAL, "Normal" },
    { DangerLevel::HOSTILE, "Hostile" }
})

class Enemy : public Entity {
    public:
        static Enemy* fromString(const std::string& code);

    public:
        explicit Enemy(std::string name);
        // enemy's attack, returns the damage dealt
        double attack() noexcept override;
        // attack the enemy, returns true if the enemy is dead
        bool attack(double dmg) noexcept override;
        std::vector<std::shared_ptr<Object>> getDrops() noexcept override;
        int getMinimumLevel() const noexcept;
        DangerLevel getMinimumDanger() const noexcept;

    private:
        int minimumLevel;
        DangerLevel minimumDanger;
};

#endif