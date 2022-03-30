// updated 29 March 2022

#include <map>
#include <string>
#include "Weapon.hpp"
#include "FileReader.hpp"
#include "AdventureException.hpp"
#include "json.hpp"
using json = nlohmann::json;

Weapon::Weapon(std::string name) {
    Object::format(name);
    if (name.size() < 1) throw AdventureException("Weapon: weapon name required");
    json data = FileReader::getFromFile("weapons.json", name);
    this->value = data["value"].get<int>();
    this->name = name;
    this->type = OBJCLASS::WEAPON;
    recipe = data["recipe"].get<std::map<std::string, unsigned int>>();
    uses = data["hitpoints"].get<int>();
    category = data["type"].get<std::string>();
    dmg = (category == "melee") ? data["m-damage"].get<double>() : data["r-damage"].get<double>();
}

Weapon::operator std::string() const noexcept {
    return "WEAPON, " + name + ", " + std::to_string(uses);
}

std::map<std::string, unsigned int> Weapon::getRecipe() const noexcept {
    return recipe;
}

int Weapon::getUses() const noexcept {
    return uses;
}

double Weapon::getDamage() const noexcept {
    return dmg;
}

void Weapon::use() {
    if (uses < 1) throw AdventureException("Weapon::use weapon is already broken");
    --uses;
}