// updated 17 June 2022
// base class to join all objects and entities

#ifndef THING_HPP
#define THING_HPP

#include <string>
#include <algorithm>

enum class Things {
    Object,
    Entity,
    Civilization
};

class Thing {    
    public:
        std::string getName() const noexcept;
        virtual unsigned int getValue() const noexcept;
        virtual Things getThingType() const noexcept = 0;
        // convert to string to allow for loading from string
        virtual operator std::string() const noexcept = 0;
        // string for printing
        virtual std::string printString() const noexcept;
    
    protected:
        std::string name;
        unsigned int value;
};

inline std::string Thing::getName() const noexcept {
    return name;
}

inline unsigned int Thing::getValue() const noexcept {
    return value;
}

inline std::string Thing::printString() const noexcept {
    return name;
}

#endif