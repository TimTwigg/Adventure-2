// updated 7 April 2022

#ifndef TOOL_HPP
#define TOOL_HPP

#include <string>
#include <map>
#include "Object.hpp"

class Tool : public Object {
    public:
        static Tool* from_string(const std::string& code);
        
    public:
        explicit Tool(std::string name);
        operator std::string() const noexcept override;
        std::map<std::string, unsigned int> getRecipe() const noexcept;
        int getUses() const noexcept;
        void use();
    
    private:
        std::map<std::string, unsigned int> recipe;
        int uses;
};

#endif