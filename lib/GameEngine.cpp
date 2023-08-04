// updated 4 August 2023

#include <string>
#include <memory>
#include <fstream>
#include <filesystem>
#include <algorithm>
#include <sstream>
#include <utility>
#include "GameEngine.hpp"
#include "Player.hpp"
#include "Map.hpp"
#include "Resource.hpp"
#include "CResource.hpp"
#include "Tool.hpp"
#include "Weapon.hpp"
#include "Container.hpp"
#include "Machine.hpp"
#include "Civilization.hpp"
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
        if (query.size() == 0) continue;
        strHelp::lower(query);
        command = strHelp::split(query);

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

        // dig
        else if (command[0] == "dig") dig();

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
    endGame();
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
    // TODO
}

void GameEngine::mine() {
    command = strHelp::reduce(command);
    if (command.size() < 4) {
        i->output("Incorrect command format: 'mine [what] [tool]'", configs["colors"]["error"].get<Color>());
        return;
    }

    // validate target
    std::string target = command[1] + " " + command[2];
    if (target == "stone deposit" || target == "metal deposit" || target == "gold deposit") {
        Location& l = map->getRef();
        if (std::find(l.miscHere.begin(), l.miscHere.end(), target) == l.miscHere.end()) {
            i->output("No " + target + " here to mine", configs["colors"]["error"].get<Color>());
            return;
        }
    }
    else {
        i->output("Can't mine a " + target, configs["colors"]["error"].get<Color>());
        return;
    }
    
    // validate tool
    std::string tool = command[3];
    if (tool == "stone-pick" || tool == "metal-pick") {
        if (!player->inInventory(tool)) {
            i->output("You don't have a " + tool + " to mine with", configs["colors"]["error"].get<Color>());
            return;
        }
    }
    else {
        i->output("You can't mine with a " + tool, configs["colors"]["error"].get<Color>());
    }

    // gold can only be mined with metal
    if (target == "gold deposit" && tool != "metal-pick") {
        i->output("You can only mine gold with a metal-pick", configs["colors"]["error"].get<Color>());
        return;
    }

    // drop object
    bool unbroken = player->use(tool);
    if (!unbroken) i->output("You broke your " + tool, configs["colors"]["output"].get<Color>());
    Location& l = map->getRef();
    if (target == "stone deposit") l.addThing(std::shared_ptr<Thing>(new Resource("stone")));
    else if (target == "metal deposit") l.addThing(std::shared_ptr<Thing>(new Resource("metal")));
    else if (target == "gold deposit") l.addThing(std::shared_ptr<Thing>(new Resource("gold")));

    player->passTime(60);
    player->reduceHT(3, 1);
}

void GameEngine::chop() {
    command = strHelp::reduce(command);
    if (command.size() < 3) {
        i->output("Incorrect command format: 'chop [what] [tool]'", configs["colors"]["error"].get<Color>());
        return;
    }

    // validate target
    std::string target = command[1];
    Location& l = map->getRef();
    if (target != "trees" || std::find(l.miscHere.begin(), l.miscHere.end(), "trees") == l.miscHere.end()) {
        i->output("No " + target + " to chop down here", configs["colors"]["error"].get<Color>());
        return;
    }

    // validate tool
    std::string tool = command[2];
    if (tool == "wood-axe" || tool == "stone-axe" || tool == "metal-axe") {
        if (!player->inInventory(tool)) {
            i->output("You don't have a " + tool, configs["colors"]["error"].get<Color>());
            return;
        }
    }
    else {
        i->output("You can't chop " + target + " with a " + tool, configs["colors"]["error"].get<Color>());
        return;
    }

    // drop object
    bool unbroken = player->use(tool);
    if (!unbroken) i->output("You broke your " + tool, configs["colors"]["output"].get<Color>());
    l.addThing(std::shared_ptr<Thing>(new Resource("wood")));

    player->passTime(60);
    player->reduceHT(3, 1);
}

void GameEngine::dig() {
    command = strHelp::reduce(command);
    if (command.size() < 2) {
        i->output("Incorrect command format: 'dig [tool]'", configs["colors"]["error"].get<Color>());
        return;
    }

    // validate target
    Location& l = map->getRef();
    if (l.biome == "ocean" || l.biome == "mountain" || l.biome == "river") {
        i->output("You can't dig in a " + l.biome, configs["colors"]["error"].get<Color>());
        return;
    }

    // validate tool
    std::string tool = command[1];
    if (tool == "wood-shovel" || tool == "stone-shovel" || tool == "metal-shovel") {
        if (!player->inInventory(tool)) {
            i->output("You don't have a " + tool, configs["colors"]["error"].get<Color>());
            return;
        }
    }
    else {
        i->output("You can't dig with a " + tool, configs["colors"]["error"].get<Color>());
        return;
    }

    // drop object
    bool unbroken = player->use(tool);
    if (!unbroken) i->output("You broke your " + tool, configs["colors"]["output"].get<Color>());
    if (l.biome == "Desert" || l.biome == "Island") l.addThing(std::shared_ptr<Thing>(new Resource("sand")));
    else l.addThing(std::shared_ptr<Thing>(new Resource("dirt")));

    player->passTime(60);
    player->reduceHT(3, 1);
}

void GameEngine::eat() {
    command = strHelp::reduce(command);
    if (command.size() < 2) {
        i->output("Eat what?", configs["colors"]["error"].get<Color>());
        return;
    }
    // validate that they have the item
    std::string target = command[1];
    if (!player->inInventory(target)) {
        i->output("You don't have a " + target, configs["colors"]["error"].get<Color>());
        return;
    }

    // validate that it is a resource
    std::shared_ptr<Object> obj = player->removeItem(target);
    if (obj->getType() != OBJCLASS::RESOURCE) {
        i->output("You can't eat a " + obj->getName(), configs["colors"]["error"].get<Color>());
        player->addItem(obj->operator std::string());
        return;
    }
    // validate that it is edible
    Resource* r = static_cast<Resource*>(obj.get());
    if (!(r->getCategory() == Category::FOOD || r->getCategory() == Category::RAW_FOOD)) {
        i->output("You can't eat a " + obj->getName(), configs["colors"]["error"].get<Color>());
        player->addItem(obj->operator std::string());
        return;
    }

    int hunger = r->getNutValue();
    player->eat(hunger);
}

void GameEngine::drink() {
    command = strHelp::reduce(command);
    if (command.size() < 3) {
        i->output("Incorrect Command Format: drink [liquid] [source]", configs["colors"]["error"].get<Color>());
        return;
    }

    std::string liq = command[1];
    std::string source = command[2];

    // validate liq is drinkable
    std::vector<std::string> liquidNames = FileReader::getTitlesFromFile("liquids.json");
    if (liq == "empty" || std::find(liquidNames.begin(), liquidNames.end(), liq) == liquidNames.end()) {
        i->output("You can't drink " + liq, configs["colors"]["error"].get<Color>());
        return;
    }

    // validate source
    if (source == "river") {
        Location& l = map->getRef();
        if (l.biome != "River" && std::find(l.miscHere.begin(), l.miscHere.end(), "river") == l.miscHere.end()) {
            i->output("No rivers here", configs["colors"]["error"].get<Color>());
            return;
        }
        else if (liq != "water") {
            i->output("There's no river of " + liq + " here", configs["colors"]["error"].get<Color>());
            return;
        }
    }
    
    else if (source == "ocean") {
        Location& l = map->getRef();
        if (l.biome != "Ocean") {
            i->output("You're not in the ocean!", configs["colors"]["error"].get<Color>());
            return;
        }
        i->output("That would make you sick", configs["colors"]["error"].get<Color>());
        return;
    }

    else {
        // source is a container
        std::vector<std::string> containers = FileReader::getTitlesFromFile("containers.json");
        if (std::find(containers.begin(), containers.end(), source) == containers.end()) {
            i->output("You can't drink " + liq + " from a " + source, configs["colors"]["error"].get<Color>());
            return;
        }
        // check player has container
        if (!player->inInventory(source, liq)) {
            i->output("You don't have a " + source + " of " + liq, configs["colors"]["error"].get<Color>());
            return;
        }

        std::shared_ptr<Object> con = player->removeContainer(source, liq);
        Container* c = static_cast<Container*>(con.get());
        c->reduce();
        player->addItem(c->operator std::string());
    }

    json liquids = FileReader::readFile("liquids.json");
    json theLiq = liquids[liq];
    player->eat(theLiq["hunger"].get<int>());
    player->drink(theLiq["thirst"].get<int>());
    player->heal(theLiq["hp"].get<int>());
    i->output("You drank " + liq + " from the " + source, configs["colors"]["info"].get<Color>());
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
            obj = static_cast<Object*>(t.get());
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
    command = strHelp::reduce(command);
    if (command.size() < 3) {
        i->output("Incorrect Command Format: raid [target] [weapon]", configs["colors"]["error"].get<Color>());
        return;
    }

    // validate target
    std::string target = command[1];
    if (!factory.inIndex(target) || factory.getTypeOf(target) != FactoryType::Civ) {
        i->output("Can't raid a " + target, configs["colors"]["error"].get<Color>());
        return;
    }

    // check if the civ type is actually present
    Location& l = map->getRef();
    Civilization* civ = nullptr;
    for (std::shared_ptr<Thing>& t : l.thingsHere) {
        if (t->getName() == target) {
            civ = static_cast<Civilization*>(t.get());
            break;
        }
    }
    if (civ == nullptr) {
        i->output("No " + target + " here to raid.", configs["colors"]["error"].get<Color>());
        return;
    }

    // check that the civ is not already raided
    if (civ->isRaided()) {
        i->output(target + " has already been raided.", configs["colors"]["output"].get<Color>());
        return;
    }

    // validate weapon
    std::string weapon = command[2];
    if (!factory.inIndex(weapon) || factory.getTypeOf(weapon) != FactoryType::Weapon) {
        i->output("Can't raid a " + target + " with a " + weapon, configs["colors"]["error"].get<Color>());
        return;
    }
    else if (!player->inInventory(weapon)) {
        i->output("You don't have a " + weapon + " to raid with.", configs["colors"]["error"].get<Color>());
        return;
    }

    // check for ammo if ranged
    const Weapon* w = static_cast<const Weapon*>(player->accessItem(weapon));
    bool isRanged = w->weaponType() == "ranged";
    if (isRanged) {
        if (!player->inInventory(w->ammoType())) {
            i->output("You don't have any " + w->ammoType(), configs["colors"]["error"].get<Color>());
            return;
        }

        // remove ammo
        player->removeItem(w->ammoType());
    }

    // use weapon
    player->use(weapon);

    // damage
    double dmg = player->attackDmg(w->getDamage());
    bool res = civ->raid(dmg);
    i->output("You raided the " + target + " for " + std::to_string(static_cast<int>(dmg)) + " damage!", configs["colors"]["output"].get<Color>());
    if (res) {
        i->output("The " + target + " has been destroyed.", configs["colors"]["output"].get<Color>());
    }
    else if (!isRanged || (gen.getRandBool() && gen.getRandBool())) {
        // counter attack
        dmg = civ->attack();
        i->output("The " + target + " counter-attacked! You take " + std::to_string(static_cast<int>(dmg)) + " damage!", configs["colors"]["output"].get<Color>());
        player->damage(dmg);
        if (player->isDead()) {
            i->output("Oh No! You Died :(", configs["colors"]["error"].get<Color>());
            endGame();
        }
    }

    // time and energy
    player->passTime(10);
    player->reduceHT(3, 1);
}

void GameEngine::loot() {
    // TODO
}

void GameEngine::trade() {
    command = strHelp::reduce(command);
    if (command.size() < 2) {
        i->output("Trade with whom?", configs["colors"]["error"].get<Color>());
        return;
    }

    // validate target
    std::string target = command[1];
    if (!factory.inIndex(target) || factory.getTypeOf(target) != FactoryType::Civ) {
        i->output("Can't trade with " + target, configs["colors"]["error"].get<Color>());
        return;
    }
    
    // check if the civ type is actually present
    Location& l = map->getRef();
    Civilization* civ = nullptr;
    for (std::shared_ptr<Thing>& t : l.thingsHere) {
        if (t->getName() == target) {
            civ = static_cast<Civilization*>(t.get());
            break;
        }
    }
    if (civ == nullptr) {
        i->output("No " + target + " here to trade with.", configs["colors"]["error"].get<Color>());
        return;
    }
    
    // trades object is alias for map<pair, pair> where the key is <type, name> and the value is <quantity, price>

    // get the trades
    trades buying = civ->getBuyingTrades();
    trades selling = civ->getSellingTrades();
    std::stringstream ss_buy;
    std::stringstream ss_sell;
    for (const auto& item : buying) {
        ss_buy << "  " << item.second.first << " " << item.first.second << ": " << item.second.second << " coins" << std::endl;
    }
    for (const auto& item : selling) {
        ss_sell << "  " << item.second.first << " " << item.first.second << ": " << item.second.second << " coins" << std::endl;
    }

    // output the trade screen
    i->clearScreen();
    i->output(ART::TRADE, configs["colors"]["art"].get<Color>());
    i->output("Buying:", configs["colors"]["headers"].get<Color>());
    i->output(ss_buy.str(), configs["colors"]["output"].get<Color>());
    i->output("Selling:", configs["colors"]["headers"].get<Color>());
    i->output(ss_sell.str(), configs["colors"]["output"].get<Color>());
    i->output("You have " + std::to_string(static_cast<int>(player->stat("wealth"))) + " coins.\n", configs["colors"]["info"].get<Color>());
    i->output(player->listInventory() + "\n", configs["colors"]["info"].get<Color>());
    i->output("Use 'buy/sell item [quantity]' to trade, 'refresh' to refresh the trade screen, and 'back/exit' to stop trading.\n", configs["colors"]["output"].get<Color>());
    
    // trade loop
    while (true) {
        // get the trades
        trades buying = civ->getBuyingTrades();
        trades selling = civ->getSellingTrades();

        // query
        std::string tquery = i->askInput("[$]", configs["colors"]["prompt"].get<Color>(), configs["colors"]["input"].get<Color>());
        strHelp::lower(tquery);
        command = strHelp::split(tquery);
        if (command.size() == 0 || command[0] == "back" || command[0] == "exit") {
            i->output("");
            break;
        }
        else if (command[0] == "refresh") {
            std::stringstream ss_buy;
            std::stringstream ss_sell;
            for (const auto& item : buying) {
                ss_buy << "  " << item.second.first << " " << item.first.second << ": " << item.second.second << " coins" << std::endl;
            }
            for (const auto& item : selling) {
                ss_sell << "  " << item.second.first << " " << item.first.second << ": " << item.second.second << " coins" << std::endl;
            }

            // output the trade screen
            i->clearScreen();
            i->output(ART::TRADE, configs["colors"]["art"].get<Color>());
            i->output("Buying:", configs["colors"]["headers"].get<Color>());
            i->output(ss_buy.str(), configs["colors"]["output"].get<Color>());
            i->output("Selling:", configs["colors"]["headers"].get<Color>());
            i->output(ss_sell.str(), configs["colors"]["output"].get<Color>());
            i->output("You have " + std::to_string(static_cast<int>(player->stat("wealth"))) + " coins.\n", configs["colors"]["info"].get<Color>());
            i->output(player->listInventory() + "\n", configs["colors"]["info"].get<Color>());
            i->output("Use 'buy/sell item [quantity]' to trade, 'refresh' to refresh the trade screen, and 'back/exit' to stop trading.\n", configs["colors"]["output"].get<Color>());
        }

        else if (command[0] == "buy") {
            if (command.size() < 2) {
                i->output("Buy what?", configs["colors"]["error"].get<Color>());
                continue;
            }
            // validate the item
            int quantity = 0;
            int cost = 0;
            std::pair<std::string, std::string> key;
            for (const auto& item : selling) {
                if (item.first.second == command[1]) {
                    quantity = item.second.first;
                    cost = item.second.second;
                    key = item.first;
                    break;
                }
            }
            if (quantity == 0) {
                i->output("This " + target + " is not selling any " + command[1], configs["colors"]["error"].get<Color>());
                continue;
            }
            // validate quantity
            int num = 1;
            if (command.size() > 2 && strHelp::isNumber(command[2])) {
                num = std::stoi(command[2]);
                if (num > quantity) num = quantity;
            }
            // validate that the player has enough coins
            if (cost * num > player->stat("wealth")) {
                i->output("You can't afford to buy " + std::to_string(num) + " " + command[1], configs["colors"]["error"].get<Color>());
                continue;
            }
            
            // make the trade
            try {
                civ->sell(key.first, key.second, num);
            } catch (AdventureException) {
                i->output("This " + target + " is not selling any " + command[1], configs["colors"]["error"].get<Color>());
                continue;
            }
            player->removeWealth(cost * num);
            Thing* thng = factory.make(key.second);
            Object* obj = static_cast<Object*>(thng);
            if (num > 1) {
                if (obj->getType() == OBJCLASS::RESOURCE || obj->getType() == OBJCLASS::CRESOURCE) {
                    Resource* r = static_cast<Resource*>(obj);
                    r->add(num-1);
                    player->addItem(r);
                }
                else {
                    player->addItem(obj);
                    for (int i = 0; i < num-1; ++i) {
                        thng = factory.make(key.second);
                        obj = static_cast<Object*>(thng);
                        player->addItem(obj);
                    }
                }
            }
            else player->addItem(obj);
            i->output("Bought " + std::to_string(num) + " " + key.second + ".\n", configs["colors"]["output"].get<Color>());
        }

        else if (command[0] == "sell") {
            if (command.size() < 2) {
                i->output("Sell what?", configs["colors"]["error"].get<Color>());
                continue;
            }

            // validate the item
            int quantity = 0;
            int cost = 0;
            std::pair<std::string, std::string> key;
            for (const auto& item : buying) {
                if (item.first.second == command[1]) {
                    quantity = item.second.first;
                    cost = item.second.second;
                    key = item.first;
                    break;
                }
            }
            if (quantity == 0) {
                i->output("This " + target + " is not buying any " + command[1], configs["colors"]["error"].get<Color>());
                continue;
            }
            // validate quantity
            int num = 1;
            if (command.size() > 2 && strHelp::isNumber(command[2])) {
                num = std::stoi(command[2]);
                if (num > quantity) num = quantity;
            }
            // check that the player has the item
            if (!player->inInventory(command[1], num)) {
                i->output("You don't have " + std::to_string(num) + " " + command[1] + " to sell!", configs["colors"]["error"].get<Color>());
                continue;
            }
            
            // make the trade
            try {
                civ->buy(key.first, key.second, num);
            } catch (AdventureException) {
                i->output("This " + target + " is not buying any " + command[1], configs["colors"]["error"].get<Color>());
                continue;
            }
            std::shared_ptr<Object> obj = player->removeItem(command[1], num);
            if (obj->getType() != OBJCLASS::RESOURCE && obj->getType() != OBJCLASS::CRESOURCE) {
                for (int i = 0; i < num-1; ++i) {
                    player->removeItem(command[1]);
                }
            }
            player->addWealth(cost * num);
            i->output("Sold " + std::to_string(num) + " " + key.second + ".\n", configs["colors"]["output"].get<Color>());
        }

        else {
            i->output("Hmmmmm", configs["colors"]["error"].get<Color>());
        }
    }
}

void GameEngine::craft() {
    command = strHelp::reduce(command);
    if (command.size() < 3) {
        i->output("Incorrect command format: craft [type] [to-craft]", configs["colors"]["error"].get<Color>());
        return;
    }

    // validate type
    std::string type = command[1];
    if (type != "object" && type != "tool" && type != "weapon" && type != "container" && type != "machine") {
        i->output("Type must be object/tool/weapon/container/machine", configs["colors"]["error"].get<Color>());
        return;
    }

    // check that they have a crafting-bench
    std::string target = command[2];
    if (!player->inInventory("crafting-bench") && target != "crafting-bench" && target != "hammer") {
        i->output("You need a crafting-bench to craft!", configs["colors"]["error"].get<Color>());
        return;
    }
    // check that they have a hammer if crafting a machine
    if (type == "machine" && !player->inInventory("hammer")) {
        i->output("You need a hammer to craft a machine!", configs["colors"]["error"].get<Color>());
        return;
    }
    // validate target
    std::string file;
    if (type == "object") file = "craftableResources.json";
    else file = type + "s.json";
    json data;
    try {
        data = FileReader::getFromFile(file, target);
    }
    catch (AdventureException e) {
        i->output("No such " + type + ": " + target, configs["colors"]["error"].get<Color>());
        return;
    }

    // validate that they have all the needed parts
    std::map<std::string, int> recipe = data["recipe"].get<std::map<std::string, int>>();
    for (auto& [k,v] : recipe) {
        if (!player->inInventory(k, v)) {
            i->output("You don't have enough " + k, configs["colors"]["error"].get<Color>());
            return;
        }
    }

    // remove the parts
    for (auto& [k,v] : recipe) {
        player->removeItem(k, v);
    }

    // add crafted item
    if (type == "object") player->addItem(OBJCLASS::CRESOURCE, target);
    else if (type == "weapon") player->addItem(OBJCLASS::WEAPON, target);
    else if (type == "tool") player->addItem(OBJCLASS::TOOL, target);
    else if (type == "container") player->addItem(OBJCLASS::CONTAINER, target);
    else if (type == "machine") player->addItem(OBJCLASS::MACHINE, target);

    player->passTime(30);
    player->reduceHT(2, 1);
}

void GameEngine::build() {
    // TODO
}

void GameEngine::recipe() {
    command = strHelp::reduce(command);
    if (command.size() < 2) {
        i->output("Recipe of what?", configs["colors"]["error"].get<Color>());
        return;
    }
    
    std::string target = command[1];
    if (!factory.inIndex(target)) {
        i->output("Could not find item: " + target, configs["colors"]["error"].get<Color>());
        return;
    }

    std::string filename = factory.getFileOf(target);
    json data = FileReader::getFromFile(filename, target);
    if (data.contains(std::string{"recipe"})) {
        std::map<std::string, int> recipe = data["recipe"].get<std::map<std::string, int>>();
        std::vector<std::string> parts;
        for (const auto& el : recipe) {
            parts.push_back(std::to_string(el.second) + " " + el.first);
        }
        std::stringstream ss;
        ss << target << ": ";
        for (int i = 0; i < parts.size()-1; ++i) {
            ss << parts[i] << ", ";
        }
        ss << parts[parts.size()-1];
        i->output(ss.str(), configs["colors"]["info"].get<Color>());
    }
    else {
        i->output(target + " has no recipe!", configs["colors"]["info"].get<Color>());
    }
}

void GameEngine::smoke() {
    command = strHelp::reduce(command);
    if (command.size() < 2) {
        i->output("Smoke what?", configs["colors"]["error"].get<Color>());
        return;
    }

    // check that they have fuel
    if (!player->inInventory("wood")) {
        i->output("You need fuel to use the smoker", configs["colors"]["error"].get<Color>());
        return;
    }

    // validate target
    std::string target = command[1];
    if (!player->inInventory(target)) {
        i->output("You don't have a " + target + " to smoke", configs["colors"]["error"].get<Color>());
        return;
    }
    // validate that target is smokable
    std::string output;
    try {
        output = FileReader::getFromFile("smoker.json", target).get<std::string>();
    }
    catch (AdventureException e) {
        i->output("You can't smoke a " + target, configs["colors"]["error"].get<Color>());
        return;
    }

    // smoke multiple items at once
    if (command.size() > 2 && command[2] == "all") {
        int count = player->itemCount(target);
        int fuelNeeded = std::ceil(static_cast<float>(count) / DEFAULTS::smoker_items_per_wood);
        if (!player->inInventory("wood", fuelNeeded)) {
            // not enough fuel for all
            i->output("Not enough wood", configs["colors"]["error"].get<Color>());
            fuelNeeded = player->itemCount("wood");
            player->addItem(OBJCLASS::RESOURCE, output, fuelNeeded*DEFAULTS::smoker_items_per_wood);
            player->removeItem("wood", fuelNeeded);
            player->removeItem(target, fuelNeeded*DEFAULTS::smoker_items_per_wood);
            player->passTime(DEFAULTS::smoker_wood_burn_time*fuelNeeded);
            player->reduceHT(fuelNeeded, fuelNeeded);
            i->output("Smoked " + std::to_string(fuelNeeded*DEFAULTS::smoker_items_per_wood) + " " + target, configs["colors"]["info"].get<Color>());
        }
        else {
            // enough fuel
            player->addItem(OBJCLASS::RESOURCE, output, count);
            player->removeItem("wood", fuelNeeded);
            player->removeItem(target, count);
            player->passTime(DEFAULTS::smoker_wood_burn_time*fuelNeeded);
            player->reduceHT(fuelNeeded, fuelNeeded);
            i->output("Smoked " + std::to_string(count) + " " + target, configs["colors"]["info"].get<Color>());
        }
    }
    else {
        player->addItem(OBJCLASS::RESOURCE, output);
        player->removeItem("wood");
        player->removeItem(target);
        player->passTime(DEFAULTS::smoker_wood_burn_time);
        player->reduceHT(1, 1);
        i->output("Smoked " + target, configs["colors"]["info"].get<Color>());
    }    
}

void GameEngine::sleep() {
    double t = player->stat("time");
    double toSleep;
    if (t > 5 && t < 20) {
        i->output("You can't sleep during the day!", configs["colors"]["error"].get<Color>());
        return;
    }
    else if (t < 5) {
        toSleep = 5 - t;
    }
    else {
        toSleep = 29 - t;
    }
    double minutes = std::floor(toSleep * 60);
    player->passTime(static_cast<int>(minutes));
    i->output("Slept for " + std::to_string(static_cast<int>(toSleep)) + " hours and " + std::to_string(static_cast<int>((toSleep - std::floor(toSleep)) * 60)) + " minutes.", configs["colors"]["info"].get<Color>());
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
    // TODO
}

void GameEngine::train() {
    // TODO
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
            while (true) {
                std::map<std::string, std::string> options = configs["colors"].get<std::map<std::string, std::string>>();
                std::vector<std::string> v;
                std::for_each(options.begin(), options.end(), [&](const auto& item){v.push_back(item.first + ": " + item.second);});
                v.push_back("Back");
                std::string answer = i->askSelect("Color Option", v);
                if (answer == "Back") break;

                std::string key = answer.substr(0, std::find(answer.begin(), answer.end(), ':')-answer.begin());
                std::string color = i->askSelect(key, {"WHITE", "BLUE_DARK", "BLUE_LIGHT", "CYAN", "GREEN_DARK", "GREEN_LIGHT",
                    "RED_DARK", "RED_LIGHT", "YELLOW", "PURPLE", "GRAY"}, configs["colors"][key].get<std::string>());
                configs["colors"][key] = color;
            }
        }

        else if (option == "Back") break;
    }
    i->clearScreen();
}

void GameEngine::endGame() {
    i->output("Thanks for playing, see you next time!", configs["colors"]["headers"].get<Color>());
    i->output(ART::LONGSWORD, configs["colors"]["art"].get<Color>());
    save();
    exit(0);
}