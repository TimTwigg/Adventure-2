// Updated: 21 January 2022

#ifndef WINDOWSINTERFACE_HPP
#define WINDOWSINTERFACE_HPP

#include <windows.h>
#include <string>
#include <vector>
#include <map>
#include "Interface.hpp"
#include "Colors.hpp"
#include "ScreenInstructions.hpp"

class WindowsInterface : public Interface {
    public:
        WindowsInterface();
        ~WindowsInterface();
        WindowsInterface(const WindowsInterface& other) = delete;
        void output(const std::string& text, Color color = Color::WHITE, bool endLine = true) override;
        std::string askSelect(const std::string& prompt, const std::vector<std::string>& options) override;
        std::string askInput(const std::string& prompt) override;
    
    private:
        void write(PCWSTR sequence);

        enum Keys {
            UP = 72,
            DOWN = 80,
            LEFT = 75,
            RIGHT = 77,
            ENTER = 13,
            ESCAPE = 27
        };
        enum class Part {
            PROMPT,
            SELECTOR,
            ANSWER,
            TEXT
        };

        int defaultColor;
        HANDLE console;
        std::map<Screen, PCWSTR> ansiIndex;
        std::map<Part, Color> colorIndex;
        std::map<Color, int> kIndex;
};

#endif