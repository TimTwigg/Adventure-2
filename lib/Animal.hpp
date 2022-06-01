// updated 31 May 2022

#ifndef ANIMAL_HPP
#define ANIMAL_HPP

#include <string>
#include <vector>
#include <memory>
#include "Entity.hpp"
#include "Object.hpp"

class Animal : public Entity {
    public:
        explicit Animal(std::string name);
        // attack player, returns damage to be dealt
        unsigned int attack() const noexcept override;
        // deal damage to the animal, returns true if the animal is dead
        bool attack(double dmg) noexcept override;
        const std::vector<std::shared_ptr<Object>>& getDrops() const noexcept override;
};

#endif