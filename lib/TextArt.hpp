// updated 12 June 2022

#ifndef TEXTART_HPP
#define TEXTART_HPP

#include <string>

namespace ART {
    const std::string SHORTSWORD =
        "      /| ____________\n"
        "O|===|* >____________>\n"
        "      \\|";

    const std::string LONGSWORD =
        "         />______________________________________\n"
        "[########[]______________________________________>\n"
        "         \\>";

    const std::string ARROW = ">>------->";

    const std::string CONFIG =
        " ______      ____             __ _           ______\n"
        "|_,.,--\\    / ___|___  _ __  / _(_) __ _    |_,.,--\\\n"
        "   ||      | |   / _ \\| '_ \\| |_| |/ _` |      ||\n"
        "   ||      | |__| (_) | | | |  _| | (_| |      ||\n"
        "   ##       \\____\\___/|_| |_|_| |_|\\__, |      ##\n"
        "   ##                              |___/       ##\n"
        "   ______________________________________________";

    const std::string SHORTBROADSWORD =
        " _         /~~> ________________________________\n"
        "/ \\////////|   \\--------------------------------\\\n"
        "\\_/\\\\\\\\\\\\\\\\|   /________________________________/\n"
        "           \\~~>";
    
    const std::string KATANA =
        "          /\\\n"
        "/vvvvvvvvvv \\-----------------------------------------,\n"
        "`^^^^^^^^^^ /========================================\"\n"
        "          \\/";
    
    const std::string DIVIDER = "#<+>-<=>-<+>-<=>-<+>-<=>-<+>-<=>-<+>-<=>-<+>-<=>-<+>#";
    
    const std::string DEFAULTPROMPT = ">>";
    
    const std::string MENUSCROLL =
        "                              ____________________\n"
        "  ________________------------                    `\\\n"
        " /:-:\\                                             |\n"
        "||< >||                     _______________________/\n"
        "|\\___/__________------------                    |\n"
        " |                TEXT ADVENTURE 2               |\n"
        " |                                               |\n"
        " |     Welcome to a text based adventure game.   |\n"
        "  |      There are a number of config options.    |\n"
        "  |     To change them, select the config option  |\n"
        "  |    on the main menu or use the \"config\"       |\n"
        "   |    command in-game.                           |\n"
        "   |   For help on how to play the game use the    |\n"
        "   |    \"help\" command.                            |\n"
        "    |                                               |\n"
        "    |         Good Luck Adventurer                  |\n"
        "    |                               ________________|__\n"
        "     |  _________________-----------                   `\\\n"
        "     |/===\\                                             |\n"
        "     ||[ ]||                            _______________/\n"
        "      \\===/__________________-----------\n";
}

#endif