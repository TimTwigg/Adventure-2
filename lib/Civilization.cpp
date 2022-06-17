// updated 16 June 2022

#include <string>
#include "Civilization.hpp"

Civilization::Civilization(std::string name) {

}

Civilization::operator std::string() const noexcept {
    return "";
}

Civilization* Civilization::fromString(const std::string& code) {
    return new Civilization("");
}