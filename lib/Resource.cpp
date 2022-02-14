// updated 14 February 2022

#include <string>
#include <algorithm>
#include "Resource.hpp"

Resource::Resource(std::string name, unsigned int count) : count{count} {
    std::transform(name.begin(), name.end(), name.begin(), [&](char c) -> char {return std::tolower(c);});
    this->name = name;
    this->type = OBJCLASS::RESOURCE;
}

Resource::operator std::string() const {
    return "RESOURCE, " + name + ", " + std::to_string(count);
}