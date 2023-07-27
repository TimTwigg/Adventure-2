// Updated 27 July 2023

#include <string>
#include <map>
#include <memory>
#include "Factory.hpp"
#include "Object.hpp"
#include "Thing.hpp"
#include "FileReader.hpp"
#include "AdventureException.hpp"
#include "Animal.hpp"
#include "Civilization.hpp"
#include "Container.hpp"
#include "Enemy.hpp"
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

std::shared_ptr<Thing> Factory::make(std::string item) {
    FactoryType ft = getTypeOf(item);

    switch (ft) {
        case FactoryType::Animal:
            return std::shared_ptr<Thing>(new Animal(item));
        case FactoryType::Civ:
            return std::shared_ptr<Thing>(new Civilization(item));
        case FactoryType::Container:
            return std::shared_ptr<Thing>(new Container(item));
        case FactoryType::CraftableResource:
            return std::shared_ptr<Thing>(new CResource(item));
        case FactoryType::Enemy:
            return std::shared_ptr<Thing>(new Enemy(item));
        case FactoryType::Machine:
            return std::shared_ptr<Thing>(new Machine(item));
        case FactoryType::Resource:
            return std::shared_ptr<Thing>(new Resource(item));
        case FactoryType::Tool:
            return std::shared_ptr<Thing>(new Tool(item));
        case FactoryType::Weapon:
            return std::shared_ptr<Thing>(new Weapon(item));
        case FactoryType::None:
            return nullptr;
    }
}