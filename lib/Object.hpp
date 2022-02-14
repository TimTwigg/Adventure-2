// Updated: 14 February 2022

#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <string>
#include "json.hpp"
using json = nlohmann::json;

enum class OBJCLASS {
    RESOURCE,
    CONTAINER,
    TOOL,
    WEAPON
};

NLOHMANN_JSON_SERIALIZE_ENUM(OBJCLASS, {
    {OBJCLASS::RESOURCE, "RESOURCE"},
    {OBJCLASS::CONTAINER, "CONTAINER"},
    {OBJCLASS::TOOL, "TOOL"},
    {OBJCLASS::WEAPON, "WEAPON"},
})

class Object {
    public:
        virtual operator std::string() const = 0; // convert object to string
        std::string getName() const noexcept;
        OBJCLASS getType() const noexcept;
    
    protected:
        std::string name;
        OBJCLASS type;
};

inline std::string Object::getName() const noexcept {
    return name;
}

inline OBJCLASS Object::getType() const noexcept {
    return type;
}

#endif