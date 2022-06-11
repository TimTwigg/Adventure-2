// updated 11 June 2022

#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <string>
#include <algorithm>
#include <vector>
#include <memory>
#include "Object.hpp"
#include "RandomGenerator.hpp"

enum class ENT {
    ANIMAL,
    ENEMY
};

namespace {
    std::string ENTtoString(ENT e) {
        switch (e) {
            case ENT::ANIMAL:
                return "ANIMAL";
            case ENT::ENEMY:
                return "ENEMY";
        }
    }
}

class Entity {
    public:
        static void strip(std::string& s) noexcept;
        static void format(std::string& s) noexcept;

    public:
        virtual ~Entity() = default;
        std::string getName() const noexcept;
        // convert to string to allow for loading from string
        virtual operator std::string() const noexcept;
        ENT getType() const noexcept;
        float getHP() const noexcept;
        unsigned int getXP() const noexcept;
        unsigned int getValue() const noexcept;
        virtual unsigned int attack() noexcept = 0;
        virtual bool attack(double dmg) noexcept = 0;
        virtual std::vector<std::shared_ptr<Object>> getDrops() noexcept = 0;
    
    protected:
        std::string name;
        ENT type;
        unsigned int maxHP;
        float hp;
        unsigned int damage;
        unsigned int xp;
        unsigned int value;
        std::vector<std::shared_ptr<Object>> drops;
        RandomGenerator gen;
};

inline Entity::operator std::string() const noexcept {
    return ENTtoString(type) + ", " + name + ", " + std::to_string(hp);
}

inline void Entity::strip(std::string& s) noexcept {
    s.erase(remove_if(s.begin(), s.end(), isspace), s.end());
}

inline void Entity::format(std::string& s) noexcept {
    std::transform(s.begin(), s.end(), s.begin(), [&](char c) -> char {return std::tolower(c);});
    Entity::strip(s);
}

inline std::string Entity::getName() const noexcept {
    return name;
}

inline ENT Entity::getType() const noexcept {
    return type;
}

inline float Entity::getHP() const noexcept {
    return hp;
}

inline unsigned int Entity::getXP() const noexcept {
    return xp;
}

inline unsigned int Entity::getValue() const noexcept {
    return value;
}

#endif