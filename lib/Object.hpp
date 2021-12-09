// Updated: 8 December 2021

#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <string>
#include <map>

class Object {
    public:
        virtual operator std::string() const = 0;
        virtual std::map<std::string, std::string> save() const = 0;
        std::string getName() const;
        std::string getType() const;
    
    private:
        std::string name;
        std::string type;
}

std::string Object::getName() const {return name;}

std::string Object::getType() const (return type;)

#endif