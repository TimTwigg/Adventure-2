// updated 30 March 2022

#ifndef LIQUID_HPP
#define LIQUID_HPP

#include <string>

class Liquid {
    public:
        Liquid();
        explicit Liquid(std::string name);
        operator std::string() const noexcept;
};

#endif