// updated 15 February 2022

#ifndef TOOL_HPP
#define TOOL_HPP

#include <string>
#include <map>
#include "Object.hpp"
#include "json.hpp"

class Tool : public Object {
    public:
        explicit Tool(std::string name);
        operator std::string() const noexcept override;
        std::map<std::string, unsigned int> getRecipe() const noexcept;
        int getUses() const noexcept;
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(Tool, type, name, uses);
    
    private:
        std::map<std::string, unsigned int> recipe;
        int uses;
};

#endif