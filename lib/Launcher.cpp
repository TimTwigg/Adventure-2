// updated 27 May 2024

#include <string>
#include <memory>
#include <map>
#include <ctime>
#include <sstream>
#include <filesystem>
#include "Launcher.hpp"
#include "Interface.hpp"
#include "TextArt.hpp"
#include "GameData.hpp"
#include "FileReader.hpp"
#include "StringHelpers.hpp"
#include "SkillSets.hpp"
#include "Player.hpp"
namespace fs = std::filesystem;

Launcher::Launcher(std::shared_ptr<Interface> i) : i{i} {
    data.configs = {
        {"prompt", ART::DEFAULTPROMPT},
        {"colors", {
            {"prompt", "GRAY"},
            {"input", "GREEN_LIGHT"},
            {"error", "RED_LIGHT"},
            {"output", "WHITE"},
            {"art", "WHITE"},
            {"info", "BLUE_LIGHT"},
            {"headers", "BLUE_DARK"}
        }}
    };
}

GameEngine* Launcher::run() {
    menu();
    return new GameEngine(i, data);
}

void Launcher::menu() {
    while (true) {
        i->clearScreen();
        i->output(ART::MENUSCROLL);
        std::string answer = i->askSelect("Menu", {"New Game", "Load Game", "Delete Save", "Config", "Quit"});
        if (answer == "New Game") {
            newGame();
            break;
        }
        else if (answer == "Load Game") {
            if (loadGame()) break;
        }
        else if (answer == "Delete Save") deleteSave();
        else if (answer == "Config") config();
        else if (answer == "Quit") exit(0);
    }
}

bool Launcher::loadGame() {
    std::vector<std::string> saves = FileReader::getSaveFileNames();
    saves.push_back("Back");
    std::string filename = i->askSelect("Choose Save", saves);
    if (filename == "Back") return false;
    
    Player* p = Player::load("saves\\" + filename);
    bool isDead = p->isDead();
    delete p;
    if (isDead) {
        i->output("Can't load save: Adventurer is dead.", Color::RED_LIGHT);
        return loadGame();
    }

    data.filename = "saves\\" + filename;
    data.isNew = false;
    return true;
}

void Launcher::newGame() {
    i->clearScreen();
    i->output(ART::NEWGAME);
    std::string filename = i->askInput("Save Name:");
    strHelp::strip(filename);
    if (filename.size() < 1) {
        // use datetime as default save name
        auto t = std::time(nullptr);
        auto tm = *std::localtime(&t);
        std::stringstream ss;
        ss << std::put_time(&tm, "%d_%m_%Y_%H_%M_%S");
        filename = "save_" + ss.str();
        i->output("Default name used: " + filename, Color::RED_DARK);
    }
    data.filename = "saves\\" + filename;
    
    // game options
    std::vector<std::string> skillsets = FileReader::getTitlesFromFile("skillsets.json");
    std::string skillset = i->askSelect("Skill Set", skillsets);
    std::string diff = i->askSelect("Difficulty", {"Easy", "Medium", "Hard"});
    std::string danger = i->askSelect("Danger", {"Peaceful", "Normal", "Hostile"});
    std::string autosave = i->askSelect("Autosave", {"Yes", "No"});

    // difficulty setting
    float diff_ratio;
    if (diff == "Easy") diff_ratio = 0.5;
    else if (diff == "Medium") diff_ratio = 1.0;
    else if (diff == "Hard") diff_ratio = 1.5;

    // autosave setting
    if (autosave == "Yes") data.configs["autosave"] = true;
    else data.configs["autosave"] = false;

    // input the settings
    data.skillset = SET::to_skillset(skillset);
    data.configs["diff"] = diff_ratio;
    data.configs["danger"] = danger;
    data.isNew = true;
}

void Launcher::deleteSave() {
    std::vector<std::string> saves = FileReader::getSaveFileNames();
    saves.push_back("Back");
    std::string filename = i->askSelect("Choose Save", saves);
    if (filename == "Back") return;
    fs::remove_all("saves\\" + filename);
}

void Launcher::config() {
    while (true) {
        i->clearScreen();
        i->output(ART::CONFIG);
        std::string option = i->askSelect("Config Options", {"Rename Save", "Back"});

        if (option == "Rename Save") {
            std::vector<std::string> saves = FileReader::getSaveFileNames();
            saves.push_back("Back");
            std::string filename = i->askSelect("Choose Save", saves);
            if (filename == "Back") continue;
            std::string newName = i->askInput("New Save Name:");
            fs::rename(fs::path("saves\\" + filename), fs::path("saves\\" + newName));
        }
        else if (option == "Back") break;
    }
}