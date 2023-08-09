// updated 9 August 2023

#ifndef ENEMY_HPP
#define ENEMY_HPP

#include <string>
#include <vector>
#include <memory>
#include "Entity.hpp"

class Enemy : public Entity {
    public:
        static Enemy* fromString(const std::string& code);

    public:
        explicit Enemy(std::string name);
        double attack() noexcept override;
        // deal damage to the enemy, returns true if the enemy is dead
        bool attack(double dmg) noexcept override;
        std::vector<std::shared_ptr<Object>> getDrops() noexcept override;
};

#endif