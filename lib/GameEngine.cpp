// updated 15 June 2022

#include <string>
#include <memory>
#include <fstream>
#include <iomanip>
#include <filesystem>
#include "GameEngine.hpp"
#include "Player.hpp"
#include "Map.hpp"
//#include "Resource.hpp"
//#include "CResource.hpp"
//#include "Tool.hpp"
//#include "Weapon.hpp"
//#include "Animal.hpp"
//#include "Enemy.hpp"
#include "Interface.hpp"
#include "FileReader.hpp"
#include "TextArt.hpp"
namespace fs = std::filesystem;

const std::vector<std::string> GameEngine::error_msgs = {
    {"I'll think about it...",
    "I don't know what you mean.",
    "What do you mean?",
    "Speak clearly!",
    "It helps to speak English."}
};

GameEngine::GameEngine(std::shared_ptr<Interface> i, GameData data) : i{i} {
    if (data.isNew) {
        player = std::shared_ptr<Player>(new Player(data.skillset, data.filename));
        map = std::shared_ptr<Map>(new Map(player->getSavepath()));
        configs = data.configs;
    }
    else {
        player = std::shared_ptr<Player>(Player::load(data.filename));
        map = std::shared_ptr<Map>(Map::load(data.filename));
        configs = FileReader::getConfigs(data.filename);
    }
}

void GameEngine::run() {
    int counter = 0;

    // mainloop
    while (true) {
        // autosave
        ++counter;
        if (counter >= 10) {
            counter = 0;
            if (configs["autosave"].get<bool>()) save();
        }

        query = i->askInput(configs["prompt"].get<std::string>());

        // look
        if (query.substr(0, 4) == "look") look();

        // inventory
        else if (query == "inventory" || query == "i") inventory();

        // go
        else if (query.substr(0, 2) == "go") go();

        // 'me'
        else if (query == "me") me();

        // hunger and thirst
        else if (query == "ht") ht();

        // health
        else if (query == "hp") hp();

        // attack
        else if (query.substr(0, 6) == "attack") attack();

        // mine
        else if (query.substr(0, 4) == "mine") mine();

        // chop
        else if (query.substr(0, 4) == "chop") chop();

        // eat
        else if (query.substr(0, 3) == "eat") eat();

        // drink
        else if (query.substr(0, 5) == "drink") drink();

        // take
        else if (query.substr(0, 4) == "take") take();

        // drop
        else if (query.substr(0, 4) == "drop") drop();

        // raid
        else if (query.substr(0, 4) == "raid") raid();

        // trade
        else if (query.substr(0, 5) == "trade") trade();

        // craft
        else if (query.substr(0, 5) == "craft") craft();

        // build
        else if (query.substr(0, 5) == "build") build();

        // recipe
        else if (query.substr(0, 6) == "recipe") recipe();

        // smoke
        else if (query.substr(0, 5) == "smoke") smoke();

        // sleep
        else if (query == "sleep") sleep();

        // wait
        else if (query.substr(0, 4) == "wait") wait();

        // time
        else if (query == "time") time();

        // fill
        else if (query.substr(0, 4) == "fill") fill();

        // train
        else if (query.substr(0, 5) == "train") train();

        // clear
        else if (query == "clear") i->clearScreen();

        // exit game
        else if (query == "exit") {
            if (configs["autosave"].get<bool>()) save();
            else {
                if (i->askYesNo("Save")) save();
            }
            break;
        }

        // save
        else if (query == "save") save();

        // help screen
        else if (query == "help") help();

        // help with commands
        else if (query[0] == '?' || query[-1] == '?') command_help();

        // help with objects
        else if (query[0] == '@') object_help();

        // config screen
        else if (query == "config") {
            i->clearScreen();
            config();
        }

        // invalid command
        else {
            int index = gen.getRandInt(0, error_msgs.size() - 1);
            i->output(error_msgs[index], Color::RED_LIGHT);
        }

        // get attacked if appropriate

    }
}

void GameEngine::look() {

}

void GameEngine::inventory() {

}

void GameEngine::go() {

}

void GameEngine::me() {

}

void GameEngine::ht() {

}

void GameEngine::hp() {

}

void GameEngine::attack() {

}

void GameEngine::mine() {

}

void GameEngine::chop() {

}

void GameEngine::eat() {

}

void GameEngine::drink() {

}

void GameEngine::take() {

}

void GameEngine::drop() {

}

void GameEngine::raid() {

}

void GameEngine::trade() {

}

void GameEngine::craft() {

}

void GameEngine::build() {

}

void GameEngine::recipe() {

}

void GameEngine::smoke() {

}

void GameEngine::sleep() {

}

void GameEngine::wait() {

}

void GameEngine::time() {

}

void GameEngine::fill() {

}

void GameEngine::train() {

}

void GameEngine::clear() {

}

void GameEngine::save() {
    player->save();
    map->save();
    
    std::string path = player->getSavepath() + "\\config.game";
    std::ofstream out;
    out.open(path);
    out << std::setw(4) << configs << std::endl;
    out.close();
}

void GameEngine::help() {

}

void GameEngine::command_help() {

}

void GameEngine::object_help() {

}

void GameEngine::config() {
    i->output(ART::CONFIG);
    while (true) {
        std::string option = i->askSelect("Config Options", {"Prompt", "Rename Save", "Autosave", "Difficulty Rating", "Back"});

        if (option == "Prompt") {
            std::string prompt = i->askSelect("Prompt Style", {ART::DEFAULTPROMPT, ART::ARROW, "Custom"});
            if (prompt == "Custom") {
                prompt = i->askInput("Custom Prompt: ");
            }
            configs["prompt"] = prompt;
        }

        else if (option == "Rename Save") {
            std::vector<std::string> saves = FileReader::getSaveFileNames();
            saves.push_back("Back");
            std::string filename = i->askSelect("Choose Save", saves);
            if (filename == "Back") continue;
            std::string newName = "saves\\" + i->askInput("New Save Name:");
            fs::rename(fs::path("saves\\" + filename), fs::path(newName));
            player->setSavePath(newName);
            map->setPath(newName);
        }

        else if (option == "Autosave") {
            configs["autosave"] = i->askYesNo("Autosave");
        }

        else if (option == "Difficulty Rating") {
            std::string answer = i->askSelect("Difficulty Setting", {"Easy", "Medium", "Hard"});
            if (answer == "Easy") configs["diff"] = 0.5;
            else if (answer == "Medium") configs["diff"] = 1.0;
            else if (answer == "Hard") configs["diff"] = 1.5;
        }

        else if (option == "Back") break;
    }
    i->clearScreen();
}