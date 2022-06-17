// updated 16 June 2022
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
        static void strip(std::string& s) noexcept;
        static void format(std::string& s) noexcept;
    
    public:
        std::string getName() const noexcept;
        virtual unsigned int getValue() const noexcept;
        // convert to string to allow for loading from string
        virtual operator std::string() const noexcept = 0;
    
    protected:
        std::string name;
        unsigned int value;
};

inline void Thing::strip(std::string& s) noexcept {
    s.erase(remove_if(s.begin(), s.end(), isspace), s.end());
}

inline void Thing::format(std::string& s) noexcept {
    std::transform(s.begin(), s.end(), s.begin(), [&](char c) -> char {return std::tolower(c);});
    Thing::strip(s);
}

inline std::string Thing::getName() const noexcept {
    return name;
}

inline unsigned int Thing::getValue() const noexcept {
    return value;
}

#endif