// Updated: 21 January 2021

#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <string>
#include <map>

class Object {
    public:
        virtual operator std::string() const = 0;
        virtual std::map<std::string, std::string> save() const = 0;
        std::string getName() const noexcept {
            return name;
        }

        std::string getType() const noexcept {
            return type;
        }
    
    private:
        std::string name;
        std::string type;
};

#endif