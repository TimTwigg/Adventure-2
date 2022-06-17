// updated 16 June 2022

#include <string>
#include <map>
#include <vector>
#include <sstream>
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

CResource::operator std::string() const noexcept {
    return "C" + Resource::operator std::string();
}

CResource* CResource::fromString(const std::string& code) {
    std::stringstream ss{code};
    std::vector<std::string> v;
    while (ss.good()) {
        std::string s;
        std::getline(ss, s, ',');
        v.push_back(s);
    }

    return new CResource(v[1], std::stoi(v[2]));
}