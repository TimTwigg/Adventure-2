// updated 16 June 2022

#include <string>
#include <memory>
#include <fstream>
#include <iomanip>
#include <filesystem>
#include <algorithm>
#include <sstream>
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
    i->clearScreen();

    // mainloop
    while (true) {
        // autosave
        ++counter;
        if (counter >= 10) {
            counter = 0;
            if (configs["autosave"].get<bool>()) save();
        }

        query = i->askInput(configs["prompt"].get<std::string>());
        std::transform(query.begin(), query.end(), query.begin(), [](unsigned char c){return std::tolower(c);});
        command = split(query);

        // look
        if (command[0] == "look") look();

        // inventory
        else if (query == "inventory" || query == "i") inventory();

        // go
        else if (command[0] == "go") go();

        // 'me'
        else if (query == "me") me();

        // hunger and thirst
        else if (query == "ht") ht();

        // health
        else if (query == "hp") hp();

        // attack
        else if (command[0] == "attack") attack();

        // mine
        else if (command[0] == "mine") mine();

        // chop
        else if (command[0] == "chop") chop();

        // eat
        else if (command[0] == "eat") eat();

        // drink
        else if (command[0] == "drink") drink();

        // take
        else if (command[0] == "take") take();

        // drop
        else if (command[0] == "drop") drop();

        // raid
        else if (command[0] == "raid") raid();

        // trade
        else if (command[0] == "trade") trade();

        // craft
        else if (command[0] == "craft") craft();

        // build
        else if (command[0] == "build") build();

        // recipe
        else if (command[0] == "recipe") recipe();

        // smoke
        else if (command[0] == "smoke") smoke();

        // sleep
        else if (query == "sleep") sleep();

        // wait
        else if (command[0] == "wait") wait();

        // time
        else if (query == "time") time();

        // fill
        else if (command[0] == "fill") fill();

        // train
        else if (command[0] == "train") train();

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

std::vector<std::string> GameEngine::split(const std::string& str) {
    std::istringstream iss{str};
    return std::vector<std::string>{std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>{}};
}

void GameEngine::printLocation(Location l) {
    std::string action, preposition, article;
    
    if (l.biome == "Ocean" || l.biome == "River") action = "swimming";
    else action = "standing";

    if (l.biome == "Mountain" || l.biome == "Plain" || l.biome == "Island") preposition = "on";
    else preposition = "in";

    // check if the first letter is a vowel.
    // method for checking taken from 
    // https://stackoverflow.com/questions/47846406/c-fastest-way-to-check-if-char-is-vowel-or-consonant
    if ((0x208222 >> (l.biome[0] & 0x1f)) & 1) article = "an";
    else article = "a";

    std::string partA = "You are " + action + " " + preposition + " " + article + " " + l.biome + ". ";

    // TODO
    /*
    if (l.here.size() == 0) i->output(partA + "There is nothing here.");
    else if (l.here.size() == 1) {

    }
    else {
        
    }*/
}

void GameEngine::look() {
    if (command.size() == 1 || command[1] == "here") printLocation(map->get());
    else if (command[1] == "north") printLocation(map->get(Dir::NORTH));
    else if (command[1] == "east") printLocation(map->get(Dir::EAST));
    else if (command[1] == "south") printLocation(map->get(Dir::SOUTH));
    else if (command[1] == "west") printLocation(map->get(Dir::WEST));
    else i->output("Couldn't recognize command, try 'look [north | east | south | west]'", Color::RED_LIGHT);
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