// Updated 25 August 2023

#ifndef FACTORY_HPP
#define FACTORY_HPP

#include <map>
#include <string>
#include <vector>
#include "Object.hpp"
#include "Thing.hpp"

enum class FactoryType {
    Animal,
    Civ,
    Container,
    CraftableResource,
    Enemy,
    Liquid,
    Machine,
    Resource,
    Storage,
    Tool,
    Weapon,
    None
};

const std::map<std::string, FactoryType> FactoryTypeMapping = {
    { "animals", FactoryType::Animal },
    { "civ-types", FactoryType::Civ },
    { "containers", FactoryType::Container },
    { "craftableResources", FactoryType::CraftableResource },
    { "enemies", FactoryType::Enemy },
    { "liquids", FactoryType::Liquid },
    { "machines", FactoryType::Machine },
    { "resources", FactoryType::Resource },
    { "storages", FactoryType::Storage },
    { "tools", FactoryType::Tool },
    { "weapons", FactoryType::Weapon }
};

const std::vector<std::string> FactoryFiles = {
    "animals", "civ-types", "containers", "craftableResources", "enemies", "liquids", "machines", "resources", "storages", "tools", "weapons"
};

class Factory {
    public:
        Factory();
        bool inIndex(std::string item);
        FactoryType getTypeOf(std::string item);
        std::string getFileOf(std::string item);
        Thing* make(std::string item);
        Thing* makeFromCode(std::string code);
    
    private:
        std::map<std::string, FactoryType> index;
};

#endif