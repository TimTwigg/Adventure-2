// updated 30 March 2022

#ifndef CONTAINER_HPP
#define CONTAINER_HPP

#include <string>
#include <map>
#include "Object.hpp"
#include "Liquid.hpp"
#include "json.hpp"

class Container : public Object {
    public:
        explicit Container(std::string name);
        Container(std::string name, Liquid content);
        operator std::string() const noexcept override;
        std::map<std::string, unsigned int> getRecipe() const noexcept;
        Liquid getContent() const noexcept;
        void reduce();
        void empty() noexcept;

    private:
        std::map<std::string, unsigned int> recipe;
        Liquid content;
        unsigned int amount;
};

#endif