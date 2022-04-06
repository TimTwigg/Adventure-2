// Updated: 6 April 2022

#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <string>
#include <algorithm>
#include "json.hpp"
using json = nlohmann::json;

enum class OBJCLASS {
    RESOURCE,
    CRESOURCE,
    CONTAINER,
    TOOL,
    WEAPON,
    LIQUID
};

NLOHMANN_JSON_SERIALIZE_ENUM(OBJCLASS, {
    {OBJCLASS::RESOURCE, "RESOURCE"},
    {OBJCLASS::CRESOURCE, "CRESOURCE"},
    {OBJCLASS::CONTAINER, "CONTAINER"},
    {OBJCLASS::TOOL, "TOOL"},
    {OBJCLASS::WEAPON, "WEAPON"},
    {OBJCLASS::LIQUID, "LIQUID"}
})

class Object {
    public:
        static void strip(std::string& s) noexcept;
        static void format(std::string& s) noexcept;

    public:
        virtual ~Object() = default;
        virtual operator std::string() const = 0; // convert object to string
        std::string getName() const noexcept;
        virtual unsigned int getValue() const noexcept;
        virtual int getWeight() const noexcept;
        OBJCLASS getType() const noexcept;
    
    protected:
        std::string name;
        unsigned int value;
        int weight;
        OBJCLASS type;
};

inline void Object::strip(std::string& s) noexcept {
    s.erase(remove_if(s.begin(), s.end(), isspace), s.end());
}

inline void Object::format(std::string& s) noexcept {
    std::transform(s.begin(), s.end(), s.begin(), [&](char c) -> char {return std::tolower(c);});
    Object::strip(s);
}

inline std::string Object::getName() const noexcept {
    return name;
}

inline unsigned int Object::getValue() const noexcept {
    return value;
}

inline int Object::getWeight() const noexcept {
    return weight;
}

inline OBJCLASS Object::getType() const noexcept {
    return type;
}

#endif