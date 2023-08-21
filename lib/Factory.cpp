// Updated 15 August 2023

#include <string>
#include <map>
#include "Factory.hpp"
#include "Object.hpp"
#include "Thing.hpp"
#include "FileReader.hpp"
#include "AdventureException.hpp"
#include "Animal.hpp"
#include "Civilization.hpp"
#include "Container.hpp"
#include "Enemy.hpp"
#include "Liquid.hpp"
#include "Resource.hpp"
#include "CResource.hpp"
#include "Tool.hpp"
#include "Weapon.hpp"
#include "Machine.hpp"
#include "json.hpp"
using json = nlohmann::json;

Factory::Factory() : index{std::map<std::string, FactoryType>()} {
    for (std::string s : FactoryFiles) {
        std::vector<std::string> data = FileReader::getTitlesFromFile(s + ".json");
        FactoryType ftype = FactoryTypeMapping.at(s);
        for (std::string item : data) {
            index[item] = ftype;
        }
    }
}

bool Factory::inIndex(std::string item) {
    return index.find(item) != index.end();
}

FactoryType Factory::getTypeOf(std::string item) {
    if (index.find(item) == index.end()) return FactoryType::None;
    return index.at(item);
}

std::string Factory::getFileOf(std::string item) {
    FactoryType ft = getTypeOf(item);

    switch (ft) {
        case FactoryType::Animal:
            return "animals.json";
        case FactoryType::Civ:
            return "civ-types.json";
        case FactoryType::Container:
            return "containers.json";
        case FactoryType::CraftableResource:
            return "craftableResources.json";
        case FactoryType::Enemy:
            return "enemies.json";
        case FactoryType::Liquid:
            return "liquids.json";
        case FactoryType::Machine:
            return "machines.json";
        case FactoryType::Resource:
            return "resources.json";
        case FactoryType::Tool:
            return "tools.json";
        case FactoryType::Weapon:
            return "weapons.json";
        case FactoryType::None:
            throw AdventureException("Item not found: " + item);
    }
}

Thing* Factory::make(std::string item) {
    FactoryType ft = getTypeOf(item);

    switch (ft) {
        case FactoryType::Animal:
            return new Animal(item);
        case FactoryType::Civ:
            return new Civilization(item);
        case FactoryType::Container:
            return new Container(item);
        case FactoryType::CraftableResource:
            return new CResource(item);
        case FactoryType::Enemy:
            return new Enemy(item);
        case FactoryType::Liquid:
            return new Liquid(item);
        case FactoryType::Machine:
            return new Machine(item);
        case FactoryType::Resource:
            return new Resource(item);
        case FactoryType::Tool:
            return new Tool(item);
        case FactoryType::Weapon:
            return new Weapon(item);
        case FactoryType::None:
            return nullptr;
    }
}