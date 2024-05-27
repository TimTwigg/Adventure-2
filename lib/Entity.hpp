// updated 26 May 2024

#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <string>
#include <vector>
#include <memory>
#include "Object.hpp"
#include "RandomGenerator.hpp"
#include "Thing.hpp"

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
            default:
                return "UNKNOWN";
        }
    }
}

class Entity : public Thing {
    public:
        virtual ~Entity() = default;
        operator std::string() const noexcept override;
        ENT getType() const noexcept;
        float getHP() const noexcept;
        unsigned int getXP() const noexcept;
        virtual double attack() noexcept = 0;
        virtual bool attack(double dmg) noexcept = 0;
        virtual std::vector<std::shared_ptr<Object>> getDrops() noexcept = 0;
    
    protected:
        ENT type;
        unsigned int maxHP;
        float hp;
        unsigned int damage;
        unsigned int xp;
        std::map<std::string, int> drops;
        RandomGenerator gen;
};

inline Entity::operator std::string() const noexcept {
    return ENTtoString(type) + ", " + name + ", " + std::to_string(hp);
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

#endif