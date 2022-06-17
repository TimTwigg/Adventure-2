// updated 17 June 2022

#ifndef CIVILIZATION_HPP
#define CIVILIZATION_HPP

#include <string>
#include "Thing.hpp"

class Civilization : public Thing {
    public:
        static Civilization* fromString(const std::string& code);

    public:
        Civilization(std::string name);
        operator std::string() const noexcept override;
        Things getThingType() const noexcept override;
};

#endif