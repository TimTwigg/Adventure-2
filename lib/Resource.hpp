// updated 17 June 2022

#ifndef RESOURCE_HPP
#define RESOURCE_HPP

#include <string>
#include "Object.hpp"
#include "json.hpp"
using json = nlohmann::json;

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
    {Category::MATERIAL, "MATERIAL"}
})

class Resource : public Object {
    public:
        static Resource* fromString(const std::string& code);

    public:
        explicit Resource(std::string name, unsigned int count = 1);
        operator std::string() const noexcept override;
        std::string printString() const noexcept override;
        Category getCategory() const noexcept;
        int getWeight() const noexcept override;
        int getCount() const noexcept;
        void add(unsigned int num = 1) noexcept;
        int getTotalValue() const noexcept;
        void remove(unsigned int num);
        int getNutValue() const noexcept;

    protected:
        unsigned int count;
        Category category;
        // nutritional value
        int nut_value;

        // default constructor protected - only for inheritance
        Resource();
};

#endif