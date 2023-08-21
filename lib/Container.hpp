// updated 21 August 2023

#ifndef CONTAINER_HPP
#define CONTAINER_HPP

#include <string>
#include <map>
#include "Object.hpp"
#include "Liquid.hpp"

class Container : public Object {
    public:
        static Container* fromString(const std::string& code);

    public:
        explicit Container(std::string name);
        Container(std::string name, Liquid content);
        operator std::string() const noexcept override;
        std::string printString() const noexcept override;
        int getWeight() const noexcept override;
        unsigned int getValue() const noexcept override;

        std::map<std::string, unsigned int> getRecipe() const noexcept;
        Liquid getContent() const noexcept;
        void reduce();
        void empty() noexcept;
        void fill(std::string liquid);
        std::string getContentName() const noexcept;
        bool isEmpty() const noexcept;

    private:
        std::map<std::string, unsigned int> recipe;
        Liquid content;
        unsigned int amount;
        unsigned int capacity;
};

#endif