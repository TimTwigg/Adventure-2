// updated 16 June 2022

#include <string>
#include <map>
#include <sstream>
#include <vector>
#include "Tool.hpp"
#include "AdventureException.hpp"
#include "FileReader.hpp"
#include "json.hpp"
using json = nlohmann::json;

Tool::Tool(std::string name) {
    Object::format(name);
    if (name.size() < 1) throw AdventureException("Tool: tool name required");
    json data = FileReader::getFromFile("tools.json", name);
    this->value = data["value"].get<int>();
    recipe = data["recipe"].get<std::map<std::string, unsigned int>>();
    uses = data["hitpoints"].get<int>();
    this->name = name;
    this->weight = data["weight"].get<int>();
    this->type = OBJCLASS::TOOL;
}

Tool::operator std::string() const noexcept {
    return "TOOL, " + name + ", " + std::to_string(uses);
}

std::map<std::string, unsigned int> Tool::getRecipe() const noexcept {
    return recipe;
}

int Tool::getUses() const noexcept {
    return uses;
}

void Tool::use() {
    if (uses < 1) throw AdventureException("Tool::use tool is already broken");
    --uses;
}

Tool* Tool::fromString(const std::string& code) {
    std::stringstream ss{code};
    std::vector<std::string> v;
    while (ss.good()) {
        std::string s;
        std::getline(ss, s, ',');
        v.push_back(s);
    }

    Tool* t = new Tool(v[1]);
    t->uses = std::stoi(v[2]);
    return t;
}