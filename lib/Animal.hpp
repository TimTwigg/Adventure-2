// updated 9 August 2023

#ifndef ANIMAL_HPP
#define ANIMAL_HPP

#include <string>
#include <vector>
#include <memory>
#include "Entity.hpp"
#include "Object.hpp"

class Animal : public Entity {
    public:
        static Animal* fromString(const std::string& code);

    public:
        explicit Animal(std::string name);
        // attack player, returns damage to be dealt
        double attack() noexcept override;
        // deal damage to the animal, returns true if the animal is dead
        bool attack(double dmg) noexcept override;
        std::vector<std::shared_ptr<Object>> getDrops() noexcept override;
};

#endif