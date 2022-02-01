// Updated: 1 February 2022

#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <string>
#include <map>

enum class OBJCLASS {
    RESOURCE,
    CONTAINER,
    TOOL,
    WEAPON
};

class Object {
    public:
        virtual operator std::string() const = 0; // convert object to string
        virtual std::map<std::string, std::string> save() const = 0;
        std::string getName() const noexcept {
            return name;
        }

        OBJCLASS getType() const noexcept {
            return type;
        }
    
    private:
        std::string name;
        OBJCLASS type;
};

#endif