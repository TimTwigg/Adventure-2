// Updated 25 August 2023

#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "Thing.hpp"
#include "json.hpp"
using json = nlohmann::json;

enum class OBJCLASS {
    RESOURCE,
    CRESOURCE,
    CONTAINER,
    TOOL,
    WEAPON,
    LIQUID,
    MACHINE,
    STORAGE
};

NLOHMANN_JSON_SERIALIZE_ENUM(OBJCLASS, {
    { OBJCLASS::RESOURCE, "RESOURCE" },
    { OBJCLASS::CRESOURCE, "CRESOURCE" },
    { OBJCLASS::CONTAINER, "CONTAINER" },
    { OBJCLASS::TOOL, "TOOL" },
    { OBJCLASS::WEAPON, "WEAPON" },
    { OBJCLASS::LIQUID, "LIQUID" },
    { OBJCLASS::MACHINE, "MACHINE" },
    { OBJCLASS::STORAGE, "STORAGE" }
})

class Object : public Thing {
    public:
        virtual ~Object() = default;
        virtual int getWeight() const noexcept;
        OBJCLASS getType() const noexcept;
    
    protected:
        int weight;
        OBJCLASS type;
};

inline int Object::getWeight() const noexcept {
    return weight;
}

inline OBJCLASS Object::getType() const noexcept {
    return type;
}

#endif