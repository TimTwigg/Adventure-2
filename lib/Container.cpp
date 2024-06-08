// updated 7 June 2024

#include <string>
#include <sstream>
#include <map>
#include <vector>
#include "Container.hpp"
#include "Liquid.hpp"
#include "FileReader.hpp"
#include "AdventureException.hpp"
#include "StringHelpers.hpp"
#include "json.hpp"
using json = nlohmann::json;

Container::Container(std::string name) {
    std::string liq = "empty";
    if (name.find("of") != std::string::npos) {
        std::vector<std::string> parts = strHelp::split(name);
        name = parts[0];
        liq = parts[2];
    }
    strHelp::format(name);
    strHelp::format(liq);

    if (name.size() < 1) throw AdventureException("Container: container name required");
    json data = FileReader::getFromFile("containers.json", name);
    this->value = data["value"].get<int>();
    this->name = name;
    this->weight = data["weight"].get<double>();
    this->type = OBJCLASS::CONTAINER;
    recipe = data["recipe"].get<std::map<std::string, unsigned int>>();
    capacity = data["amount"].get<unsigned int>();
    content = Liquid(liq);
    amount = 0;
}

Container::Container(std::string name, Liquid contents) : Container{name} {
    content = contents;
}

Container::operator std::string() const noexcept {
    return "CONTAINER, " + name + ", " + static_cast<std::string>(content) + ", " + std::to_string(amount);
}

std::string Container::printString() const noexcept {
    return (getContentName() == "empty") ? "empty " + name : name + " of " + getContentName();
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
    if (amount < 1) empty();
}

void Container::empty() noexcept {
    amount = 0;
    content = Liquid();
}

void Container::fill(std::string liquid) {
    amount = capacity;
    content = Liquid(liquid);
}

Container* Container::fromString(const std::string& code) {
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
    Container* c = new Container(name, content);
    c->amount = amount;

    return c;
}

std::string Container::getContentName() const noexcept {
    return content.getName();
}

bool Container::isEmpty() const noexcept {
    return content.getName() == "empty";
}