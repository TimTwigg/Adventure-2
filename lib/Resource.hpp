// updated 14 February 2022

#ifndef RESOURCE_HPP
#define RESOURCE_HPP

#include <string>
#include "Object.hpp"

class Resource : public Object {
    public:
        explicit Resource(std::string name, unsigned int count = 1);
        operator std::string() const override;
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(Resource, type, name, count);

    private:
        unsigned int count;
};

#endif