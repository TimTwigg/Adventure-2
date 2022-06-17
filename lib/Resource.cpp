// updated 17 June 2022

#include <string>
#include <algorithm>
#include <sstream>
#include <vector>
#include "Resource.hpp"
#include "AdventureException.hpp"
#include "FileReader.hpp"
#include "StringHelpers.hpp"
#include "json.hpp"
using json = nlohmann::json;

Resource::Resource(std::string name, unsigned int count) : count{count} {
    strHelp::format(name);
    if (name.size() < 1) throw AdventureException("Resource: resource name required");
    json data = FileReader::getFromFile("resources.json", name);
    category = data["category"].get<Category>();
    nut_value = data["general1"].get<int>();
    this->value = data["value"].get<int>();
    this->name = name;
    this->weight = data["weight"].get<int>();
    this->type = OBJCLASS::RESOURCE;
}

Resource::operator std::string() const noexcept {
    return "RESOURCE, " + name + ", " + std::to_string(count);
}

std::string Resource::printString() const noexcept {
    if (count < 2) return Thing::printString();
    else return std::to_string(count) + " " + name + "s";
}

Category Resource::getCategory() const noexcept {
    return category;
}

int Resource::getWeight() const noexcept {
    return weight * count;
}

int Resource::getCount() const noexcept {
    return count;
}

void Resource::add(unsigned int num) noexcept {
    count += num;
}

int Resource::getTotalValue() const noexcept {
    return value * count;
}

void Resource::remove(unsigned int num) {
    if (num > count) throw AdventureException("Resource::remove not enough to reduce by " + std::to_string(num));
    count -= num;
}

int Resource::getNutValue() const noexcept {
    return nut_value;
}

Resource::Resource() {}

Resource* Resource::fromString(const std::string& code) {
    std::stringstream ss{code};
    std::vector<std::string> v;
    while (ss.good()) {
        std::string s;
        std::getline(ss, s, ',');
        v.push_back(s);
    }

    return new Resource(v[1], std::stoi(v[2]));
}