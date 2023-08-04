// updated 4 August 2023

#ifndef WEAPON_HPP
#define WEAPON_HPP

#include <string>
#include <map>
#include "Object.hpp"

class Weapon : public Object {
    public:
        static Weapon* fromString(const std::string& code);

    public:
        explicit Weapon(std::string name);
        operator std::string() const noexcept override;
        std::map<std::string, unsigned int> getRecipe() const noexcept;
        int getUses() const noexcept;
        double getDamage() const noexcept;
        std::string weaponType() const noexcept;
        std::string ammoType() const noexcept;
        void use();
    
    private:
        std::map<std::string, unsigned int> recipe;
        int uses;
        double dmg;
        std::string category;
        std::string ammo;
};

#endif