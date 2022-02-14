// updated 14 February 2022

#include <string>
#include <algorithm>
#include "Tool.hpp"

Tool::Tool(std::string name) {
    std::transform(name.begin(), name.end(), name.begin(), [&](char c) -> char {return std::tolower(c);});
    this->name = name;
    this->type = OBJCLASS::TOOL;
}

Tool::operator std::string() const {
    return "TOOL, " + name;
}