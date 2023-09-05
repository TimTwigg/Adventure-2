// updated 25 August 2023

#ifndef LIQUID_HPP
#define LIQUID_HPP

#include <string>
#include "Object.hpp"

class Liquid : public Object {
    public:
        static Liquid* fromString(const std::string& code);

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