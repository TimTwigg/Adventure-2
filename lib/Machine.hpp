// updated 14 September 2022

#ifndef MACHINE_HPP
#define MACHINE_HPP

#include <string>
#include <map>
#include "Object.hpp"

class Machine : public Object {
    public:
        static Machine* fromString(const std::string& code);

    public:
        Machine(std::string name);
        operator std::string() const noexcept override;
        bool fueled() const noexcept;

    private:
        bool requiresFuel;
};

#endif