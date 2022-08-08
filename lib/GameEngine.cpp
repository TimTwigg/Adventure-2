// updated 7 August 2022

#include <string>
#include <memory>
#include <fstream>
#include <filesystem>
#include <algorithm>
#include "GameEngine.hpp"
#include "Player.hpp"
#include "Map.hpp"
#include "Resource.hpp"
#include "CResource.hpp"
#include "Tool.hpp"
#include "Weapon.hpp"
#include "Container.hpp"
//#include "Animal.hpp"
//#include "Enemy.hpp"
#include "Interface.hpp"
#include "FileReader.hpp"
#include "StringHelpers.hpp"
#include "StatDefaults.hpp"
#include "TextArt.hpp"
#include "AdventureException.hpp"
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

        query = i->askInput(configs["prompt"].get<std::string>(), configs["colors"]["prompt"].get<Color>(), configs["colors"]["input"].get<Color>());
        if (query.size() > 0) {
            strHelp::lower(query);
            command = strHelp::split(query);
        }

        // look
        if (command[0] == "look" || query == "l") look();

        // inventory
        else if (command[0] == "inventory" || command[0] == "i") inventory();

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

        // loot
        else if (command[0] == "loot") loot();

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
        else if (query[0] == '?' || query[query.size()-1] == '?') command_help();

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
            i->output(error_msgs[index], configs["colors"]["error"].get<Color>());
            continue;
        }

        // get attacked if appropriate

    }
    i->output("Goodbye", Color::BLUE_LIGHT);
}

std::string GameEngine::chooseArticle(const std::string& s) {
    if (strHelp::isVowel(s[0])) return "an";
    else return "a";
}

void GameEngine::printLocation(Location l, bool farOff) {
    std::string partA;
    if (farOff) {
        partA = "There is " + chooseArticle(l.biome) + " " + l.biome + ". ";
    }
    else {
        std::string action, preposition;
    
        if (l.biome == "Ocean" || l.biome == "River") action = "swimming";
        else action = "standing";

        if (l.biome == "Mountain" || l.biome == "Plain" || l.biome == "Island") preposition = "on";
        else preposition = "in";

        partA = "You are " + action + " " + preposition + " " + chooseArticle(l.biome) + " " + l.biome + ". ";
    }

    std::vector<std::string> here;
    std::for_each(l.miscHere.begin(), l.miscHere.end(), [&](const std::string& s){here.push_back(s);});
    std::for_each(l.thingsHere.begin(), l.thingsHere.end(), [&](const std::shared_ptr<Thing>& p){here.push_back(p->printString());});

    std::string partB;
    if (farOff) partB = " there.";
    else partB = " here.";

    if (here.size() == 0) i->output(partA + "There is nothing" + partB, configs["colors"]["output"].get<Color>());
    else if (here.size() == 1) {
        const std::string& thing = here[0]; 
        if (thing[thing.size()-1] == 's') i->output(partA + "There are " + thing + partB, configs["colors"]["output"].get<Color>());
        else i->output(partA + "There is " + chooseArticle(here[0]) + " " + thing + partB, configs["colors"]["output"].get<Color>());
    }
    else {
        std::string list = here[0];
        std::string article;
        if (list[list.size()-1] == 's') article = "are";
        else article = "is";

        std::for_each(here.begin()+1, here.end()-1, [&](const std::string& s){
            if (s[s.size()-1] == 's') list = list + ", " + s;
            else list = list + ", a " + s;
        });
        const std::string& thing = here[here.size()-1];
        if (thing[thing.size()-1] == 's') list = list + ", and " + thing;
        else list = list + ", and a " + thing;
        i->output(partA + "There " + article + " " + list + partB, configs["colors"]["output"].get<Color>());
    }
}

void GameEngine::look() {
    if (command.size() == 1 || command[1] == "here") printLocation(map->get());
    else if (command[1] == "north") printLocation(map->get(Dir::NORTH), true);
    else if (command[1] == "east") printLocation(map->get(Dir::EAST), true);
    else if (command[1] == "south") printLocation(map->get(Dir::SOUTH), true);
    else if (command[1] == "west") printLocation(map->get(Dir::WEST), true);
    else i->output("Could not recognize command, try 'look [north | east | south | west]'", configs["colors"]["error"].get<Color>());
}

void GameEngine::inventory() {
    if (command.size() > 1) {
        i->output(player->inInventory(command[1]) ? "  Yes" : "  No", configs["colors"]["info"].get<Color>());
    }
    else i->output("  Inventory      | " + player->listInventory(), configs["colors"]["info"].get<Color>());
}

void GameEngine::go() {
    if (player->weight() > player->stat("carry_weight")) {
        i->output("You are carrying too much. Drop something to move", configs["colors"]["error"].get<Color>());
        return;
    }

    if (command[1] == "north") printLocation(map->go(Dir::NORTH));
    else if (command[1] == "east") printLocation(map->go(Dir::EAST));
    else if (command[1] == "south") printLocation(map->go(Dir::SOUTH));
    else if (command[1] == "west") printLocation(map->go(Dir::WEST));
    else {
        i->output("Could not recognize command, try 'go [north | east | south | west]'", configs["colors"]["error"].get<Color>());
        return;
    }
    player->passTime((DEFAULTS::travel_time / player->stat("speed")));
    player->reduceHT(3, 2);
}

void GameEngine::me() {
    i->output(player->getMe(), configs["colors"]["info"].get<Color>());
}

void GameEngine::ht() {
    i->output(player->getHT(), configs["colors"]["info"].get<Color>());
}

void GameEngine::hp() {
    i->output(player->getHP(), configs["colors"]["info"].get<Color>());
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
    command = strHelp::reduce(command);
    if (command.size() < 2) {
        i->output("What do you want to take?", configs["colors"]["error"].get<Color>());
        return;
    }
    std::string target = command[1];
    Location& l = map->getRef();
    Object* obj = nullptr;
    for (std::shared_ptr<Thing>& t : l.thingsHere) {
        if (t->getName() == target) {
            obj = dynamic_cast<Object*>(t.get());
            break;
        }
    }
    if (obj == nullptr) i->output("No " + target + " to take here", configs["colors"]["error"].get<Color>());
    else {
        if (obj->getType() == OBJCLASS::RESOURCE || obj->getType() == OBJCLASS::CRESOURCE) {
            int num = 1;
            if (command.size() > 2 && strHelp::isNumber(command[2])) num = std::stoi(command[2]);
            Resource* r = static_cast<Resource*>(obj);
            if (r->getCount() <= num) {
                player->addItem(r->getType(), r->getName(), r->getCount());
                l.thingsHere.erase(std::find_if(l.thingsHere.begin(), l.thingsHere.end(), [&](std::shared_ptr<Thing> i){return i->getName() == r->getName();}));
            }
            else {
                r->remove(num);
                player->addItem(r->getType(), r->getName(), num);
            }
        }
        else {
            player->addItem(obj->getType(), obj->getName());
            l.thingsHere.erase(std::find_if(l.thingsHere.begin(), l.thingsHere.end(), [&](std::shared_ptr<Thing> i){return i->getName() == obj->getName();}));
        }
    }
}

void GameEngine::drop() {
    command = strHelp::reduce(command);
    if (command.size() < 2) {
        i->output("Drop what?", configs["colors"]["error"].get<Color>());
        return;
    }
    std::string target = command[1];
    int num = 1;
    if (command.size() > 2 && strHelp::isNumber(command[2])) num = std::stoi(command[2]);
    if (player->inInventory(target, num)) {
        Location& l = map->getRef();
        l.addThing(player->removeItem(target, num));
    }
    else i->output("You don't have " + target + " to drop!", configs["colors"]["error"].get<Color>());
}

void GameEngine::raid() {

}

void GameEngine::loot() {
    
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
    command = strHelp::reduce(command);
    if (command.size() < 2) i->output("Wait how long?", configs["colors"]["error"].get<Color>());
    else if (!strHelp::isNumber(command[1])) i->output("Wait time must be number not " + command[1], configs["colors"]["error"].get<Color>());
    else {
        float n = std::stof(command[1]);
        player->passTime(n * 60);
    }
}

void GameEngine::time() {
    double t = player->stat("time");
    int d = player->stat("days");
    int hours = t;
    t = (t - hours) * 60;
    int mins = t;
    std::string hourString = std::to_string(hours);
    if (hourString.size() == 1) hourString = "0" + hourString;
    std::string minString = std::to_string(mins);
    if (minString.size() == 1) minString = "0" + minString;
    i->output("It is " + hourString + ":" + minString + " on day " + std::to_string(d), configs["colors"]["info"].get<Color>());
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
    out << configs.dump(4) << std::endl;
    out.close();

    i->output("Game Saved!", configs["colors"]["output"].get<Color>());
}

void GameEngine::help() {
    i->clearScreen();
    i->output(ART::HELP, configs["colors"]["art"].get<Color>());
    std::string content = FileReader::getInfoData("help.txt");
    i->output(content, configs["colors"]["info"].get<Color>());
}

void GameEngine::command_help() {
    std::string s = command[0];
    s.erase(std::remove(s.begin(), s.end(), '?'), s.end());
    json jdata;
    try {
        jdata = FileReader::getFromFile("commandUses.json", s, FileReader::infodatapath);
    }
    catch (AdventureException e) {
        i->output("No Command Found: " + s, configs["colors"]["error"].get<Color>());
        return;
    }
    std::vector<std::string> data = jdata.get<std::vector<std::string>>();
    i->output("Command: " + s + "\n  Usage: " + data[0], configs["colors"]["info"].get<Color>());
    if (data.size() > 1) std::for_each(data.begin()+1, data.end(), [&](const std::string& line){i->output("  " + line, configs["colors"]["info"].get<Color>());});
}

void GameEngine::object_help() {
    std::string s = command[0];
    s.erase(s.begin());
    json data;
    try {
        data = FileReader::getFromFile("object_info.json", s, FileReader::infodatapath);
    }
    catch (AdventureException e) {
        i->output("No Object Found: " + s, configs["colors"]["error"].get<Color>());
        return;
    }
    i->output("Object: " + s, configs["colors"]["info"].get<Color>());
    std::vector<std::string> v = data.get<std::vector<std::string>>();
    std::for_each(v.begin(), v.end(), [&](const std::string& str){i->output("  " + str, configs["colors"]["info"].get<Color>());});
}

void GameEngine::config() {
    i->output(ART::CONFIG, configs["colors"]["art"].get<Color>());
    while (true) {
        std::string option = i->askSelect("Config Options", {"Prompt", "Rename Save", "Autosave", "Difficulty Rating", "Colors", "Back"});

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
            std::string answer = i->askSelect("Difficulty Setting", {"Easy", "Medium", "Hard"}, configs["diff"].get<std::string>());
            if (answer == "Easy") configs["diff"] = 0.5;
            else if (answer == "Medium") configs["diff"] = 1.0;
            else if (answer == "Hard") configs["diff"] = 1.5;
        }

        else if (option == "Colors") {
            std::map<std::string, std::string> options = configs["colors"].get<std::map<std::string, std::string>>();
            std::vector<std::string> v;
            std::for_each(options.begin(), options.end(), [&](const auto& item){v.push_back(item.first + ": " + item.second);});
            std::string answer = i->askSelect("Color Option", v);

            std::string key = answer.substr(0, std::find(answer.begin(), answer.end(), ':')-answer.begin());
            std::string color = i->askSelect(key, {"WHITE", "BLUE_DARK", "BLUE_LIGHT", "CYAN", "GREEN_DARK", "GREEN_LIGHT",
                "RED_DARK", "RED_LIGHT", "YELLOW", "PURPLE", "GRAY"}, configs["colors"][key].get<std::string>());
            configs["colors"][key] = color;
        }

        else if (option == "Back") break;
    }
    i->clearScreen();
}