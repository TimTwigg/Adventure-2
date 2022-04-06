// updated 6 April 2022

#ifndef LIQUID_HPP
#define LIQUID_HPP

#include <string>
#include "Object.hpp"

class Liquid : public Object {
    public:
        explicit Liquid(std::string name = "empty");
        operator std::string() const noexcept override;
        int getHunger() const noexcept;
        int getThirst() const noexcept;
        int getHP() const noexcept;

    private:
        int hunger;
        int thirst;
        int hp;
};

#endif