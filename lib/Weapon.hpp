// updated 6 April 2022

#ifndef WEAPON_HPP
#define WEAPON_HPP

#include <string>
#include <map>
#include "Object.hpp"

class Weapon : public Object {
    public:
        explicit Weapon(std::string name);
        operator std::string() const noexcept override;
        std::map<std::string, unsigned int> getRecipe() const noexcept;
        int getUses() const noexcept;
        double getDamage() const noexcept;
        void use();
    
    private:
        std::map<std::string, unsigned int> recipe;
        int uses;
        double dmg;
        std::string category;
};

#endif