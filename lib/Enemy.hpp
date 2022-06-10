// updated 9 June 2022

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
        unsigned int attack() noexcept override;
        bool attack(double dmg) noexcept override;
        std::vector<std::shared_ptr<Object>> getDrops() noexcept override;
};

#endif