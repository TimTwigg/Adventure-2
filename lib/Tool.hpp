// updated 14 February 2022

#ifndef TOOL_HPP
#define TOOL_HPP

#include <string>
#include "Object.hpp"
#include "json.hpp"
using json = nlohmann::json;

class Tool : public Object {
    public:
        explicit Tool(std::string name);
        operator std::string() const override;
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(Tool, type, name);
};

#endif