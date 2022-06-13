// updated 12 June 2022

#include <string>
#include <memory>
#include <map>
#include "Launcher.hpp"
#include "Interface.hpp"
#include "TextArt.hpp"
#include "GameData.hpp"

Launcher::Launcher(std::shared_ptr<Interface> i) : i{i} {
}

GameEngine* Launcher::run() {
    menu();
    data.filename = "testing";
    data.isNew = true;
    data.skillset = SkillSets::TRAVELER;
    return new GameEngine(i, data);
}

void Launcher::menu() {
    while (true) {
        i->clearScreen();
        i->output(ART::MENUSCROLL);
        std::string answer = i->askSelect("Menu", {"New Game", "Load Game", "Config", "Quit"});
        if (answer == "New Game") newGame();
        else if (answer == "Load Game") loadGame();
        else if (answer == "Config") config();
        else if (answer == "Quit") break;
    }
}

void Launcher::loadGame() {

}

void Launcher::newGame() {

}

void Launcher::config() {
    while (true) {
        i->clearScreen();
        i->output(ART::CONFIG);
        std::string option = i->askSelect("Config Options", {"Prompt", "Back"});

        if (option == "Prompt") {
            std::string prompt = i->askSelect("Prompt Style", {ART::DEFAULTPROMPT, ART::ARROW, "Custom"});
            if (prompt == "Custom") {
                prompt = i->askInput("Custom Prompt");
            }
            data.configs["prompt"] = prompt;
        }
        else if (option == "Back") break;
    }
}