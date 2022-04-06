// updated 6 April 2022

#include <string>
#include <map>
#include "CResource.hpp"
#include "AdventureException.hpp"
#include "FileReader.hpp"
#include "json.hpp"
using json = nlohmann::json;

CResource::CResource(std::string name, unsigned int count) {
    Object::format(name);
    if (name.size() < 1) throw AdventureException("Resource: resource name required");
    json data = FileReader::getFromFile("craftableResources.json", name);
    this->category = data["category"].get<Category>();
    this->value = data["value"].get<int>();
    this->name = name;
    this->weight = data["weight"].get<int>();
    this->type = OBJCLASS::RESOURCE;
    this->count = count;
    recipe = data["recipe"].get<std::map<std::string, unsigned int>>();
}

std::map<std::string, unsigned int> CResource::getRecipe() const noexcept {
    return recipe;
}