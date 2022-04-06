// updated 6 April 2022

#include <string>
#include <sstream>
#include <map>
#include <vector>
#include "Container.hpp"
#include "Liquid.hpp"
#include "FileReader.hpp"
#include "AdventureException.hpp"
#include "json.hpp"
using json = nlohmann::json;

Container::Container(std::string name) {
    Object::format(name);
    if (name.size() < 1) throw AdventureException("Container: container name required");
    json data = FileReader::getFromFile("containers.json", name);
    this->value = data["value"].get<int>();
    this->name = name;
    this->weight = data["weight"].get<double>();
    this->type = OBJCLASS::CONTAINER;
    recipe = data["recipe"].get<std::map<std::string, unsigned int>>();
    amount = data["amount"].get<unsigned int>();
    content = Liquid();
}

Container::Container(std::string name, Liquid contents) : Container{name} {
    content = contents;
}

Container::operator std::string() const noexcept {
    return "CONTAINER, " + name + ", " + static_cast<std::string>(content) + ", " + std::to_string(amount);
}

int Container::getWeight() const noexcept {
    return weight + content.getWeight() * amount;
}

unsigned int Container::getValue() const noexcept {
    return value + content.getValue() * amount;
}

std::map<std::string, unsigned int> Container::getRecipe() const noexcept {
    return recipe;
}

Liquid Container::getContent() const noexcept {
    return content;
}

void Container::reduce() {
    if (amount < 1) throw AdventureException("Container::reduce container already empty");
    --amount;
}

void Container::empty() noexcept {
    amount = 0;
    content = Liquid();
}

Container Container::fromString(std::string code) {
    std::stringstream ss{code};
    std::vector<std::string> v;
    while (ss.good()) {
        std::string s;
        std::getline(ss, s, ',');
        v.push_back(s);
    }

    std::string name = v[1];
    Liquid content{v[3]};
    int amount = std::stoi(v[4]);
    Container c{name, content};
    c.amount = amount;

    return c;
}