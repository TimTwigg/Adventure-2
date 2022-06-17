// updated 16 June 2022

#ifndef CRESOURCE_HPP
#define CRESOURCE_HPP

#include <string>
#include <map>
#include "Resource.hpp"

class CResource : public Resource {
    public:
        static CResource* fromString(const std::string& code);

    public:
        explicit CResource(std::string name, unsigned int count = 1);
        std::map<std::string, unsigned int> getRecipe() const noexcept;
        operator std::string() const noexcept override;
    
    private:
        std::map<std::string, unsigned int> recipe;
};

#endif