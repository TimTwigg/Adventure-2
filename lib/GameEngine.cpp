// updated 7 June 2024

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
#include "Tool.hpp"
#include "Weapon.hpp"
#include "Container.hpp"
#include "Machine.hpp"
#include "Civilization.hpp"
#include "Entity.hpp"
#include "Storage.hpp"
//#include "Animal.hpp"
//#include "Enemy.hpp"
#include "Interface.hpp"
#include "FileReader.hpp"
#include "StringHelpers.hpp"
#include "Defaults.hpp"
#include "TextArt.hpp"
#include "AdventureException.hpp"
#include "Config.hpp"
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

    int autoSaveInterval = gameConfig.getIntOption("AutoSaveInterval");

    // mainloop
    while (true) {
        // autosave
        ++counter;
        if (counter >= autoSaveInterval) {
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

        // store
        else if (command[0] == "store") store();

        // retrieve (item from storage)
        else if (command[0] == "retrieve") retrieve();

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

        // smelt
        else if (command[0] == "smelt") smelt();

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
        // TODO

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
        else {
            article = "is " + chooseArticle(here[0]);
        }

        std::for_each(here.begin()+1, here.end()-1, [&](const std::string& s){
            if (s[s.size()-1] == 's') list = list + ", " + s;
            else list = list + ", a " + s;
        });
        const std::string& thing = here[here.size()-1];
        if (thing[thing.size()-1] == 's') list = list + ", and " + thing;
        else list = list + ", and " + chooseArticle(thing) + " " + thing;
        i->output(partA + "There " + article + " " + list + partB, configs["colors"]["output"].get<Color>());
    }
}

void GameEngine::look() {
    if (command.size() == 1 || command[1] == "here") printLocation(map->get());
    else if (command[1] == "north") {
        printLocation(map->get(Dir::NORTH), true);
        if (command.size() == 3 && command[2] == "binoculars") {
            if (!player->inInventory("binoculars")) {
                i->output("You don't have any binoculars!", configs["colors"]["error"].get<Color>());
                return;
            }
            i->output("To the far North: ", configs["colors"]["output"].get<Color>(), false);
            printLocation(map->get(Dir::NORTH, 2), true);
        }
    }
    else if (command[1] == "east") {
        printLocation(map->get(Dir::EAST), true);
        if (command.size() == 3 && command[2] == "binoculars") {
            if (!player->inInventory("binoculars")) {
                i->output("You don't have any binoculars!", configs["colors"]["error"].get<Color>());
                return;
            }
            i->output("To the far East: ", configs["colors"]["output"].get<Color>(), false);
            printLocation(map->get(Dir::EAST, 2), true);
        }
    }
    else if (command[1] == "south") {
        printLocation(map->get(Dir::SOUTH), true);
        if (command.size() == 3 && command[2] == "binoculars") {
            if (!player->inInventory("binoculars")) {
                i->output("You don't have any binoculars!", configs["colors"]["error"].get<Color>());
                return;
            }
            i->output("To the far South: ", configs["colors"]["output"].get<Color>(), false);
            printLocation(map->get(Dir::SOUTH, 2), true);
        }
    }
    else if (command[1] == "west") {
        printLocation(map->get(Dir::WEST), true);
        if (command.size() == 3 && command[2] == "binoculars") {
            if (!player->inInventory("binoculars")) {
                i->output("You don't have any binoculars!", configs["colors"]["error"].get<Color>());
                return;
            }
            i->output("To the far West: ", configs["colors"]["output"].get<Color>(), false);
            printLocation(map->get(Dir::WEST, 2), true);
        }
    }
    else if (command[1] == "around") {
        printLocation(map->get());
        i->output("To the North: ", configs["colors"]["output"].get<Color>(), false);
        printLocation(map->get(Dir::NORTH), true);
        i->output("To the East: ", configs["colors"]["output"].get<Color>(), false);
        printLocation(map->get(Dir::EAST), true);
        i->output("To the South: ", configs["colors"]["output"].get<Color>(), false);
        printLocation(map->get(Dir::SOUTH), true);
        i->output("To the West: ", configs["colors"]["output"].get<Color>(), false);
        printLocation(map->get(Dir::WEST), true);
    }
    else i->output("Could not recognize command, try 'look [here | north | east | south | west | around]'", configs["colors"]["error"].get<Color>());
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

    if (command[1] == "north") printLocation(map->go(Dir::NORTH, configs["danger"].get<DangerLevel>(), player->stat("level")));
    else if (command[1] == "east") printLocation(map->go(Dir::EAST, configs["danger"].get<DangerLevel>(), player->stat("level")));
    else if (command[1] == "south") printLocation(map->go(Dir::SOUTH, configs["danger"].get<DangerLevel>(), player->stat("level")));
    else if (command[1] == "west") printLocation(map->go(Dir::WEST, configs["danger"].get<DangerLevel>(), player->stat("level")));
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
    command = strHelp::reduce(command);
    if (command.size() < 3) {
        i->output("Incorrect command format: attack [target] [weapon]", configs["colors"]["error"].get<Color>());
        return;
    }

    // validate target
    std::string target = command[1];
    FactoryType ftype = factory.getTypeOf(target);
    if (ftype != FactoryType::Animal && ftype != FactoryType::Enemy) {
        i->output("You can't attack a " + target, configs["colors"]["error"].get<Color>());
        return;
    }
    Location& l = map->getRef();
    std::shared_ptr<Thing> t = l.accessThing(target);
    if (t == nullptr) {
        i->output("No " + target + " here to attack.", configs["colors"]["error"].get<Color>());
        return;
    }
    Entity* e = static_cast<Entity*>(t.get());

    // validate weapon
    std::string weapon = command[2];
    bool isRanged;
    double dmg;
    if (weapon == "fist") {
        dmg = player->fistDmg();
        isRanged = false;
    }
    else {
        if (factory.getTypeOf(weapon) != FactoryType::Weapon) {
            i->output("Can't attack a " + target + " with a " + weapon, configs["colors"]["error"].get<Color>());
            return;
        }
        else if (!player->inInventory(weapon)) {
            i->output("You don't have a " + weapon + " to attack with.", configs["colors"]["error"].get<Color>());
            return;
        }
    
        // check for ammo if ranged
        const Weapon* w = static_cast<const Weapon*>(player->readItem(weapon));
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

        // dmg
        dmg = player->attackDmg(w->getDamage());
    }
    
    bool res = e->attack(dmg);
    i->output("You attacked the " + target + " for " + std::to_string(static_cast<int>(dmg)) + " damage!", configs["colors"]["output"].get<Color>());
    if (res) {
        // killed the target
        i->output("You killed the " + target, configs["colors"]["output"].get<Color>());
        player->addXP(e->getXP());
        // add the drops
        std::vector<std::shared_ptr<Object>> drops = e->getDrops();
        for (auto& o : drops) l.addThing(o);
        // remove the target
        l.removeThing(e->getName());
    }
    else if (!isRanged || (gen.getRandBool() && gen.getRandBool())) {
        // counter attack
        dmg = e->attack();
        i->output("The " + target + " counter-attacked! You take " + std::to_string(static_cast<int>(dmg)) + " damage!", configs["colors"]["output"].get<Color>());
        player->damage(dmg);
        if (player->isDead()) {
            i->output("Oh No! You Died :(", configs["colors"]["error"].get<Color>());
            endGame();
        }
    }

    // time and energy
    player->passTime(5);
    player->reduceHT(3, 1);
    player->addXP(5);
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
    else if (target == "metal deposit") l.addThing(std::shared_ptr<Thing>(new Resource("metal-ore")));
    else if (target == "gold deposit") l.addThing(std::shared_ptr<Thing>(new Resource("gold-ore")));

    player->passTime(60);
    player->reduceHT(3, 1);
    player->addXP(5);
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
    player->addXP(5);
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
    player->addXP(5);
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
            if (obj->getType() == OBJCLASS::STORAGE) {
                Storage* s = static_cast<Storage*>(obj);
                if (!s->isCarryable()) {
                    i->output("Can't pick up a " + target, configs["colors"]["error"].get<Color>());
                    return;
                }
            }
            player->addItem(static_cast<Object*>(factory.makeFromCode(obj->operator std::string())));
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

void GameEngine::store() {
    command = strHelp::reduce(command);
    if (command.size() < 3) {
        i->output("Incorrect Command Format: store [object] [storage]", configs["colors"]["error"].get<Color>());
        return;
    }

    // validate object
    std::string obj = command[1];
    if (!player->inInventory(obj)) {
        i->output("You don't have a " + obj, configs["colors"]["error"].get<Color>());
        return;
    }

    // validate storage
    std::string storage = command[2];
    Storage* s = nullptr;
    if (factory.getTypeOf(storage) != FactoryType::Storage) {
        i->output("You can't store a " + obj + " in a " + storage, configs["colors"]["error"].get<Color>());
        return;
    }
    json data = FileReader::getFromFile("storages.json", storage);
    if (data["carry"].get<bool>()) {
        if (!player->inInventory(storage)) {
            i->output("You don't have a " + storage, configs["colors"]["error"].get<Color>());
            return;
        }
        s = static_cast<Storage*>(player->accessItem(storage));
    }
    else {
        Location& l = map->getRef();
        for (std::shared_ptr<Thing>& t : l.thingsHere) {
            if (t->getName() == storage) {
                s = static_cast<Storage*>(t.get());
                break;
            }
        }
        if (s == nullptr) {
            i->output("There's no " + storage + " here to store your " + obj, configs["colors"]["error"].get<Color>());
            return;
        }
    }

    s->store(player->removeItem(obj));
    i->output("Stored " + obj + " in the " + storage, configs["colors"]["output"].get<Color>());
}

void GameEngine::retrieve() {
    command = strHelp::reduce(command);
    if (command.size() < 3) {
        i->output("Incorrect Command Format: retrieve [object] [storage]", configs["colors"]["error"].get<Color>());
        return;
    }

    // validate storage
    std::string storage = command[2];
    Storage* s = nullptr;
    if (factory.getTypeOf(storage) != FactoryType::Storage) {
        i->output("You can't retrieve items from a " + storage, configs["colors"]["error"].get<Color>());
        return;
    }
    json data = FileReader::getFromFile("storages.json", storage);
    if (data["carry"].get<bool>()) {
        if (!player->inInventory(storage)) {
            i->output("You don't have a " + storage, configs["colors"]["error"].get<Color>());
            return;
        }
        s = static_cast<Storage*>(player->accessItem(storage));
    }
    else {
        Location& l = map->getRef();
        for (std::shared_ptr<Thing>& t : l.thingsHere) {
            if (t->getName() == storage) {
                s = static_cast<Storage*>(t.get());
                break;
            }
        }
        if (s == nullptr) {
            i->output("There's no " + storage + " here", configs["colors"]["error"].get<Color>());
            return;
        }
    }

    // validate object
    std::string obj = command[1];
    if (!s->contains(obj)) {
        i->output("There's no " + obj + " in the " + storage, configs["colors"]["error"].get<Color>());
        return;
    }

    auto o = s->remove(obj);
    player->addItem(static_cast<Object*>(factory.makeFromCode(o->operator std::string())));
    i->output("Retrieved the " + obj + " from the " + storage, configs["colors"]["output"].get<Color>());
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
        i->output(target + " has already been raided.", configs["colors"]["error"].get<Color>());
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
    const Weapon* w = static_cast<const Weapon*>(player->readItem(weapon));
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
        player->addXP(civ->getXP());
    }
    else if (!isRanged || (gen.getRandBool() && gen.getRandBool())) {
        // counter attack
        dmg = civ->attack();
        if (dmg > 0) {
            i->output("The " + target + " counter-attacked! You take " + std::to_string(static_cast<int>(dmg)) + " damage!", configs["colors"]["output"].get<Color>());
            player->damage(dmg);
            if (player->isDead()) {
                i->output("Oh No! You Died :(", configs["colors"]["error"].get<Color>());
                endGame();
            }
        }
    }

    // time and energy
    player->passTime(10);
    player->reduceHT(3, 1);
    player->addXP(10);
}

void GameEngine::loot() {
    command = strHelp::reduce(command);
    // validate target
    std::string target = command[1];
    if (!factory.inIndex(target) || factory.getTypeOf(target) != FactoryType::Civ) {
        i->output("Can't loot a " + target, configs["colors"]["error"].get<Color>());
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
        i->output("No " + target + " here to loot.", configs["colors"]["error"].get<Color>());
        return;
    }

    // check that the civ is already raided but not looted
    if (!civ->isRaided()) {
        i->output(target + " has not been raided yet.", configs["colors"]["error"].get<Color>());
        return;
    }
    else if (civ->isLooted()) {
        i->output(target + " has already been looted.", configs["colors"]["error"].get<Color>());
        return;
    }

    // add loot items to map
    std::map<std::pair<std::string, std::string>, int> loot = civ->getLoot();
    std::string output;
    if (loot.size() > 0) {
        std::vector<std::string> itemStrings;
        std::for_each(loot.begin(), loot.end(), [&](const std::pair<std::pair<std::string, std::string>, int>& item){
            if (item.first.first == "wealth") {
                itemStrings.push_back("treasure hoard");
                player->addWealth(item.second);
            }
            else if (item.second > 1 && (item.first.first == "resources" || item.first.first == "cresources")) {
                Thing* t = factory.make(item.first.second);
                Resource* r = static_cast<Resource*>(t);
                r->add(item.second-1);
                itemStrings.push_back(r->printString());
                l.addThing(std::shared_ptr<Thing>(r));
            }
            else {
                for (int i = 0; i < item.second; ++i) {
                    Thing* t = factory.make(item.first.second);
                    itemStrings.push_back(t->printString());
                    l.addThing(std::shared_ptr<Thing>(t));
                }
            }
        });

        // output
        std::string article;
        if (itemStrings[0][itemStrings[0].size()-1] == 's') article = "";
        else if (strHelp::isVowel(itemStrings[0][0])) article = "an ";
        else article = "a ";

        if (itemStrings.size() == 1) output = "You found a few coins, and " + article + itemStrings[0] + ".";
        else {
            std::string list = itemStrings[0];
            std::for_each(itemStrings.begin()+1, itemStrings.end()-1, [&](const std::string& s){
                if (s[s.size()-1] == 's') list = list + ", " + s;
                else if (strHelp::isVowel(s[0])) list = list + ", an " + s;
                else list = list + ", a " + s;
            });
            const std::string& thing = itemStrings[itemStrings.size()-1];
            if (thing[thing.size()-1] == 's') list = list + ", and " + thing;
            else list = list + ", and a " + thing;
            output = "You found a few coins, " + article + list + ".";
        }
    }
    else output = "You found nothing except a few coins.";

    player->addWealth(civ->getWealth());
    i->output(output, configs["colors"]["output"].get<Color>());
    player->passTime(120);
    player->reduceHT(5, 2);
    player->addXP(5);
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
    if (command.size() < 2) {
        i->output("Incorrect command format: craft [to-craft]", configs["colors"]["error"].get<Color>());
        return;
    }

    // validate type
    std::string target = command[1];
    FactoryType type = factory.getTypeOf(target);
    if (type == FactoryType::None) {
        i->output("What is " + chooseArticle(target) + " " + target + "?", configs["colors"]["error"].get<Color>());
        return;
    }
    if (type != FactoryType::Container && type != FactoryType::CraftableResource && type != FactoryType::Machine && type != FactoryType::Tool && type != FactoryType::Weapon && type != FactoryType::Storage) {
        i->output("Type must be object/tool/weapon/container/machine", configs["colors"]["error"].get<Color>());
        return;
    }

    // check that they have a crafting-bench
    if (!player->inInventory("crafting-bench") && target != "crafting-bench" && target != "hammer") {
        i->output("You need a crafting-bench to craft!", configs["colors"]["error"].get<Color>());
        return;
    }
    // check that they have a hammer if crafting a machine
    if (type == FactoryType::Machine && !player->inInventory("hammer")) {
        i->output("You need a hammer to craft a machine!", configs["colors"]["error"].get<Color>());
        return;
    }
    // validate target
    std::string file = factory.getFileOf(target);
    json data;
    try {
        data = FileReader::getFromFile(file, target);
    }
    catch (AdventureException e) {
        i->output("No such object: " + target, configs["colors"]["error"].get<Color>());
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
    if (type == FactoryType::CraftableResource) player->addItem(OBJCLASS::CRESOURCE, target);
    else if (type == FactoryType::Weapon) player->addItem(OBJCLASS::WEAPON, target);
    else if (type == FactoryType::Tool) player->addItem(OBJCLASS::TOOL, target);
    else if (type == FactoryType::Container) player->addItem(OBJCLASS::CONTAINER, target);
    else if (type == FactoryType::Machine) player->addItem(OBJCLASS::MACHINE, target);
    else if (type == FactoryType::Storage) {
        if (data["carry"].get<bool>()) player->addItem(OBJCLASS::STORAGE, target);
        else {
            Location& l = map->getRef();
            l.addThing(std::shared_ptr<Thing>(factory.make(target)));
        }
    }

    i->output("Crafted " + target, configs["colors"]["output"].get<Color>());
    player->passTime(30);
    player->reduceHT(2, 1);
    player->addXP(1);
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

    // check that they have a smoker
    if (!player->inInventory("smoker")) {
        i->output("You don't have a smoker!", configs["colors"]["error"].get<Color>());
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
        output = FileReader::getFromFile("smoker.json", target, "outputs").get<std::string>();
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
            player->addXP(3*fuelNeeded);
            i->output("Smoked " + std::to_string(fuelNeeded*DEFAULTS::smoker_items_per_wood) + " " + target, configs["colors"]["info"].get<Color>());
        }
        else {
            // enough fuel
            player->addItem(OBJCLASS::RESOURCE, output, count);
            player->removeItem("wood", fuelNeeded);
            player->removeItem(target, count);
            player->passTime(DEFAULTS::smoker_wood_burn_time*fuelNeeded);
            player->reduceHT(fuelNeeded, fuelNeeded);
            player->addXP(3*fuelNeeded);
            i->output("Smoked " + std::to_string(count) + " " + target, configs["colors"]["info"].get<Color>());
        }
    }
    else {
        player->addItem(OBJCLASS::RESOURCE, output);
        player->removeItem("wood");
        player->removeItem(target);
        player->passTime(DEFAULTS::smoker_wood_burn_time);
        player->reduceHT(1, 1);
        player->addXP(3);
        i->output("Smoked " + target, configs["colors"]["info"].get<Color>());
    }
}

void GameEngine::smelt() {
    command = strHelp::reduce(command);
    if (command.size() < 2) {
        i->output("Smelt what?", configs["colors"]["error"].get<Color>());
        return;
    }

    // check that they have a furnace
    if (!player->inInventory("furnace")) {
        i->output("You don't have a furnace!", configs["colors"]["error"].get<Color>());
        return;
    }

    // check that they have fuel
    if (!player->inInventory("wood")) {
        i->output("You need fuel to use the furnace", configs["colors"]["error"].get<Color>());
        return;
    }

    // validate target
    std::string target = command[1];
    if (!player->inInventory(target)) {
        i->output("You don't have a " + target + " to smelt", configs["colors"]["error"].get<Color>());
        return;
    }
    // validate that target is smeltable
    std::string output;
    try {
        output = FileReader::getFromFile("furnace.json", target, "outputs").get<std::string>();
    }
    catch (AdventureException e) {
        i->output("You can't smelt a " + target, configs["colors"]["error"].get<Color>());
        return;
    }

    // smoke multiple items at once
    if (command.size() > 2 && command[2] == "all") {
        int count = player->itemCount(target);
        int fuelNeeded = std::ceil(static_cast<float>(count) / DEFAULTS::furnace_items_per_wood);
        if (!player->inInventory("wood", fuelNeeded)) {
            // not enough fuel for all
            i->output("Not enough wood", configs["colors"]["error"].get<Color>());
            fuelNeeded = player->itemCount("wood");
            player->addItem(OBJCLASS::RESOURCE, output, fuelNeeded*DEFAULTS::furnace_items_per_wood);
            player->removeItem("wood", fuelNeeded);
            player->removeItem(target, fuelNeeded*DEFAULTS::furnace_items_per_wood);
            player->passTime(DEFAULTS::furnace_wood_burn_time*fuelNeeded);
            player->reduceHT(fuelNeeded, fuelNeeded);
            player->addXP(3*fuelNeeded);
            i->output("Smelted " + std::to_string(fuelNeeded*DEFAULTS::furnace_items_per_wood) + " " + target, configs["colors"]["info"].get<Color>());
        }
        else {
            // enough fuel
            player->addItem(OBJCLASS::RESOURCE, output, count);
            player->removeItem("wood", fuelNeeded);
            player->removeItem(target, count);
            player->passTime(DEFAULTS::furnace_wood_burn_time*fuelNeeded);
            player->reduceHT(fuelNeeded, fuelNeeded);
            player->addXP(3*fuelNeeded);
            i->output("Smelted " + std::to_string(count) + " " + target, configs["colors"]["info"].get<Color>());
        }
    }
    else {
        player->addItem(OBJCLASS::RESOURCE, output);
        player->removeItem("wood");
        player->removeItem(target);
        player->passTime(DEFAULTS::furnace_wood_burn_time);
        player->reduceHT(1, 1);
        player->addXP(3);
        i->output("Smelted " + target, configs["colors"]["info"].get<Color>());
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
    command = strHelp::reduce(command);
    if (command.size() < 4) {
        i->output("Incorrect command format: fill [container] [liquid] [source]", configs["colors"]["error"].get<Color>());
        return;
    }

    // validate the container
    std::string container = command[1];
    if (factory.getTypeOf(container) != FactoryType::Container) {
        i->output("You can't fill a " + container, configs["colors"]["error"].get<Color>());
        return;
    }
    if (!player->inInventory(container)) {
        i->output("You don't have a " + container, configs["colors"]["error"].get<Color>());
        return;
    }

    // validate the liquid
    std::string liquid = command[2];
    if (factory.getTypeOf(liquid) != FactoryType::Liquid) {
        i->output("You can't fill a " + container + " with " + liquid, configs["colors"]["error"].get<Color>());
        return;
    }

    // validate the source
    std::string source = command[3];
    if (liquid == "water") {
        Location loc = map->get();
        // ocean
        if (source == "ocean" && loc.biome == "Ocean") {
            i->output("You can't drink from the ocean!", configs["colors"]["error"].get<Color>());
            return;
        }
        // river(s)
        else if (source == "river") {
            if (loc.biome != "River" && std::find(loc.miscHere.begin(), loc.miscHere.end(), "river") == loc.miscHere.end()) {
                i->output("There's no river to fill up from here.", configs["colors"]["error"].get<Color>());
                return;
            }
        }
        else {
            i->output("You can't fill up with water from a " + source, configs["colors"]["error"].get<Color>());
            return;
        }
    }
    // the requested liquid is not water.
    // other liquids are not accessible in the wild
    else {
        i->output("No " + liquid + " to fill up with here.", configs["colors"]["error"].get<Color>());
        return;
    }

    // find container
    std::vector<Object*> objects = player->getAll(container);
    std::vector<Container*> containers;
    std::for_each(objects.begin(), objects.end(), [&](Object* o){containers.push_back(static_cast<Container*>(o));});
    Container* cont = nullptr;
    for (auto c : containers) {
        if (c->getName() == container && c->isEmpty()) {
            cont = c;
            break;
        }
    }
    if (cont == nullptr) {
        for (auto c : containers) {
            if (c->getName() == container && c->getContentName() == liquid) {
                cont = c;
                break;
            }
        }
    }
    if (cont == nullptr) {
        i->output("All of your " + container + "s are full with other liquids!", configs["colors"]["error"].get<Color>());
        return;
    }

    cont->fill(liquid);
    i->output("You filled your " + container + " with " + liquid, configs["colors"]["output"].get<Color>());
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
        std::string option = i->askSelect("Config Options", {"Prompt", "Rename Save", "Autosave", "Difficulty Rating", "Danger Rating", "Colors", "Back"});

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

        else if (option == "Danger Rating") {
            configs["danger"] = i->askSelect("Danger Setting", {"Peaceful", "Normal", "Hostile"}, configs["danger"].get<std::string>());
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