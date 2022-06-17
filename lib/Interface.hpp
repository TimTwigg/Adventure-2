// Updated: 17 June 2022
#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#include <string>
#include <vector>
#include "Colors.hpp"

class Interface {
    public:
        virtual void output(const std::string& text, Color color = Color::WHITE, bool endLine = true) = 0;
        virtual std::string askSelect(const std::string& prompt, const std::vector<std::string>& options, const std::string& intial = "") = 0;
        virtual bool askYesNo(const std::string& prompt) = 0;
        virtual std::string askInput(const std::string& prompt, Color promptC = Color::GRAY, Color inputC = Color::GREEN_LIGHT) = 0;
        virtual void clearScreen() = 0;
        virtual void wait() = 0;
};

#endif