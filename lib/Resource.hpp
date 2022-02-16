// updated 15 February 2022

#ifndef RESOURCE_HPP
#define RESOURCE_HPP

#include <string>
#include "Object.hpp"
#include "json.hpp"

enum class Category {
    FOOD,
    RAW_FOOD,
    OBJECT,
    AMMO,
    MATERIAL
};

NLOHMANN_JSON_SERIALIZE_ENUM(Category, {
    {Category::FOOD, "FOOD"},
    {Category::RAW_FOOD, "RAW_FOOD"},
    {Category::OBJECT, "OBJECT"},
    {Category::AMMO, "AMMO"},
    {Category::MATERIAL, "MATERIAL"},
})

class Resource : public Object {
    public:
        explicit Resource(std::string name, unsigned int count = 1);
        operator std::string() const noexcept override;
        Category getCategory() const noexcept;
        int getWeight() const noexcept;
        int getCarryWeight() const noexcept;
        int getCount() const noexcept;
        void add(unsigned int num = 1) noexcept;
        int getTotalValue() const noexcept;
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(Resource, type, name, count);

    private:
        unsigned int count;
        Category category;
        int weight;
};

#endif