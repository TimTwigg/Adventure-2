// Updated: 21 January 2022

#include <string>
#include <iostream>
#include <windows.h>
#include <vector>
#include <conio.h>
#include "WindowsInterface.hpp"
#include "Colors.hpp"

WindowsInterface::WindowsInterface() : console{GetStdHandle(STD_OUTPUT_HANDLE)} {
    ansiIndex = {
        {Screen::CURSOR_UP, L"\x1B[A"},
        {Screen::CURSOR_DOWN, L"\x1B[B"},
        {Screen::CURSOR_RIGHT, L"\x1B[C"},
        {Screen::CURSOR_LEFT, L"\x1B[D"},
        {Screen::CURSOR_HOME, L"\x1B[H"},
        {Screen::CLEAR_SCREEN, L"\x1B[2J"},
        {Screen::CLEAR_LINE, L"\x1B[2K"}
    };

    colorIndex = {
        {Part::PROMPT, Color::GREEN_LIGHT},
        {Part::SELECTOR, Color::BLUE_LIGHT},
        {Part::ANSWER, Color::BLUE_DARK},
        {Part::TEXT, Color::WHITE}
    };

    kIndex = {
        {Color::WHITE, 7},
        {Color::BLUE_DARK, 1},
        {Color::BLUE_LIGHT, 3},
        {Color::CYAN, 11},
        {Color::GREEN_DARK, 2},
        {Color::GREEN_LIGHT, 10},
        {Color::RED_DARK, 4},
        {Color::RED_LIGHT, 12},
        {Color::YELLOW, 6},
        {Color::PURPLE, 13},
        {Color::GRAY, 8}
    };

    CONSOLE_SCREEN_BUFFER_INFO buffer;
    GetConsoleScreenBufferInfo(console, &buffer);
    defaultColor = buffer.wAttributes;
}

WindowsInterface::~WindowsInterface() {
}

void WindowsInterface::output(const std::string& text, Color color, bool endLine) {
    if (color == Color::WHITE) {
        std::cout << text;
        if (endLine) std::cout << std::endl;
        return;
    }

    SetConsoleTextAttribute(console, kIndex[color]);
    std::cout << text;
    if (endLine) std::cout << std::endl;
    SetConsoleTextAttribute(console, defaultColor);
}

std::string WindowsInterface::askSelect(const std::string& prompt, const std::vector<std::string>& options) {
    write(ansiIndex[Screen::CLEAR_SCREEN]);
    output(prompt, colorIndex[Part::PROMPT]);
    int current = 0;
    int size = options.size();
    for (int i = 0; i < size; ++i) {
        if (i == current) output(" >> ", colorIndex[Part::SELECTOR], false);
        else output("    ", colorIndex[Part::TEXT], false);
        output(options[i]);
    }

    int c = 0;
    bool selected = false;
    while (!selected) {
        switch((c = getch())) {
            case Keys::UP:
                if (current > 0) --current;
                break;
            case Keys::DOWN:
                if (current < size - 1) ++current;
                break;
            case Keys::ENTER:
                selected = true;
                break;
            case Keys::ESCAPE:
                current = 0;
                selected = true;
                break;
        }
        for (int i = 0; i < size; ++i) {
            write(ansiIndex[Screen::CURSOR_UP]);
            write(ansiIndex[Screen::CLEAR_LINE]);
        }
        for (int i = 0; i < size; ++i) {
            if (i == current) output(" >> ", colorIndex[Part::SELECTOR], false);
            else output("    ", colorIndex[Part::TEXT], false);
            output(options[i]);
        }
    }

    return options[current];
}

std::string WindowsInterface::askInput(const std::string& prompt) {
    output(prompt + " ", colorIndex[Part::PROMPT], false);
    SetConsoleTextAttribute(console, kIndex[colorIndex[Part::ANSWER]]);
    std::string answer;
    std::getline(std::cin, answer);
    SetConsoleTextAttribute(console, defaultColor);
    return answer;
}

void WindowsInterface::write(PCWSTR sequence) {
    DWORD mode;
    GetConsoleMode(console, &mode);
    const DWORD originalMode = mode;
    mode |= 0x0004; // Enables Virtual Terminal Processing
    SetConsoleMode(console, mode);
    DWORD written;
    WriteConsoleW(console, sequence, (DWORD)wcslen(sequence), &written, NULL);
    SetConsoleMode(console, originalMode);
}