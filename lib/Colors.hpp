// Updated: 17 June 2022

#ifndef COLORS_HPP
#define COLORS_HPP

#include "json.hpp"

enum class Color {
    WHITE,
    BLUE_DARK,
    BLUE_LIGHT,
    CYAN,
    GREEN_DARK,
    GREEN_LIGHT,
    RED_DARK,
    RED_LIGHT,
    YELLOW,
    PURPLE,
    GRAY
};

NLOHMANN_JSON_SERIALIZE_ENUM(Color, {
    {Color::WHITE, "WHITE"},
    {Color::BLUE_DARK, "BLUE_DARK"},
    {Color::BLUE_LIGHT, "BLUE_LIGHT"},
    {Color::CYAN, "CYAN"},
    {Color::GREEN_DARK, "GREEN_DARK"},
    {Color::GREEN_LIGHT, "GREEN_LIGHT"},
    {Color::RED_DARK, "RED_DARK"},
    {Color::RED_LIGHT, "RED_LIGHT"},
    {Color::YELLOW, "YELLOW"},
    {Color::PURPLE, "PURPLE"},
    {Color::GRAY, "GRAY"}
})

#endif