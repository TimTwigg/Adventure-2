// updated 17 June 2022

#include <string>
#include "Civilization.hpp"

Civilization::Civilization(std::string name) {
    this->name = name;

}

Civilization::operator std::string() const noexcept {
    return "CIVILIZATION, " + name;
}

Civilization* Civilization::fromString(const std::string& code) {
    return new Civilization("house");
}

Things Civilization::getThingType() const noexcept {
    return Things::Civilization;
}