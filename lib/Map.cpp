// updated 9 August 2023

#include <map>
#include <vector>
#include <string>
#include <utility>
#include <algorithm>
#include <fstream>
#include <filesystem>
#include "Map.hpp"
#include "RandomGenerator.hpp"
#include "FileReader.hpp"
#include "AdventureException.hpp"
#include "Animal.hpp"
#include "Enemy.hpp"
#include "Civilization.hpp"
#include "Container.hpp"
#include "CResource.hpp"
#include "Resource.hpp"
#include "Tool.hpp"
#include "Weapon.hpp"
#include "Machine.hpp"
#include "json.hpp"
using json = nlohmann::json;
namespace fs = std::filesystem;

Location::Location() {}

Location::Location(RandomGenerator& gen) {
    json data = FileReader::readFile("biomes.json");
    std::vector<std::string> biomes;
    for (auto [k, _] : data.items()) biomes.push_back(k);
    int index = gen.getRandInt(0, biomes.size()-1);
    biome = biomes[index];
    data = data[biome];

    if (biome == "Forest" || biome == "Jungle" || (data["trees"].get<bool>() && gen.getRandBool())) {
        miscHere.push_back("trees");
        if (biome == "Forest" || biome == "Jungle" || gen.getRandBool()) {
            int count = gen.getRandInt(0, 3);
            if (count > 0) thingsHere.push_back(std::shared_ptr<Thing>(new Resource("wood", count)));
        }
    }

    if (biome == "Mountain" || (data["stone"].get<bool>() && gen.getRandBool())) {
        miscHere.push_back("stone deposit");
        if (biome == "Mountain" || gen.getRandBool()) {
            int count = gen.getRandInt(0, 3);
            if (count > 0) thingsHere.push_back(std::shared_ptr<Thing>(new Resource("stone", count)));
        }
    }

    if (data["metal"].get<bool>() && gen.getRandBool()) miscHere.push_back("metal deposit");
    if (data["gold"].get<bool>() && gen.getRandBool()) miscHere.push_back("gold deposit");
    if (biome != "River" && gen.getRandBool() && gen.getRandInt(0, 10) * data["river"].get<double>() >= gen.getRandInt(1, 10)) miscHere.push_back("river");
    
    std::vector<std::string> civs = data["civ-types"].get<std::vector<std::string>>();
    if (civs.size() > 0 && gen.getRandBool() && gen.getRandBool()) thingsHere.push_back(std::shared_ptr<Thing>(new Civilization(civs[gen.getRandInt(0, civs.size()-1)])));
    
    std::vector<std::string> animals = data["animals"].get<std::vector<std::string>>();
    if (animals.size() > 0) {
        for (const std::string& a : animals) {
            if (gen.getRandBool()) thingsHere.push_back(std::shared_ptr<Thing>(new Animal(a)));
        }
    }
    
    std::vector<std::string> enemies = data["enemies"].get<std::vector<std::string>>();
    if (enemies.size() > 0) {
        for (const std::string& e : enemies) {
            // TODO: uncomment to add enemies into the game.
            // Should they only start appearing at a certain level? Some other marker?
            /////////////////////////
            //if (gen.getRandBool() && gen.getRandBool()) thingsHere.push_back(std::shared_ptr<Thing>(new Enemy(e)));
            /////////////////////////
        }
    }
}

Location::Location(const std::string& biome, json here) : biome{biome} {
    for (auto& j : here) {
        if (j.is_object()) thingsHere.push_back(std::shared_ptr<Thing>(Civilization::load(j)));
        else {
            std::string code = j.get<std::string>();
            std::string type = code.substr(0, std::find(code.begin(), code.end(), ',') - code.begin());
            if (type == "ANIMAL") thingsHere.push_back(std::shared_ptr<Thing>(Animal::fromString(code)));
            else if (type == "ENEMY") thingsHere.push_back(std::shared_ptr<Thing>(Enemy::fromString(code)));
            else if (type == "CONTAINER") thingsHere.push_back(std::shared_ptr<Thing>(Container::fromString(code)));
            else if (type == "CRESOURCE") thingsHere.push_back(std::shared_ptr<Thing>(CResource::fromString(code)));
            else if (type == "RESOURCE") thingsHere.push_back(std::shared_ptr<Thing>(Resource::fromString(code)));
            else if (type == "TOOL") thingsHere.push_back(std::shared_ptr<Thing>(Tool::fromString(code)));
            else if (type == "WEAPON") thingsHere.push_back(std::shared_ptr<Thing>(Weapon::fromString(code)));
            else if (type == "MACHINE") thingsHere.push_back(std::shared_ptr<Thing>(Machine::fromString(code)));
            else miscHere.push_back(code);
        }
    }
}

json Location::save() const {
    json j;
    std::for_each(miscHere.begin(), miscHere.end(), [&](std::string s){j.push_back(s);});
    std::for_each(thingsHere.begin(), thingsHere.end(), [&](std::shared_ptr<Thing> p){
        Civilization* c = dynamic_cast<Civilization*>(p.get());
        if (c != nullptr) j.push_back(c->save());
        else j.push_back(p->operator std::string());
    });
    return j;
}

std::shared_ptr<Thing> Location::accessThing(std::string name) {
    std::shared_ptr<Thing> p = nullptr;
    for (std::shared_ptr<Thing>& t : thingsHere) {
        if (t->getName() == name) {
            p = t;
            break;
        }
    }
    return p;
}

void Location::addThing(std::shared_ptr<Thing> thing) {
    if (dynamic_cast<Object*>(thing.get()) != nullptr) {
        Object* obj = static_cast<Object*>(thing.get());
        OBJCLASS type = obj->getType();
        if (type == OBJCLASS::RESOURCE || type == OBJCLASS::CRESOURCE) {
            bool exists = false;
            for (std::shared_ptr<Thing>& it : thingsHere) {
                if (it->getName() == obj->getName()) {
                    Resource* r = static_cast<Resource*>(obj);
                    int count = r->getCount();
                    Resource* rit = static_cast<Resource*>(it.get());
                    rit->add(count);
                    exists = true;
                    break;
                }
            }
            if (!exists) thingsHere.push_back(thing);
        }
        else thingsHere.push_back(thing);
    }
    else thingsHere.push_back(thing);
}

void Location::removeThing(std::string thing) {
    std::vector<std::shared_ptr<Thing>>::iterator it;
    for (it = thingsHere.begin(); it != thingsHere.end(); ++it) {
        if (it->get()->getName() == thing) break;
    }
    thingsHere.erase(it);
}

Map::Map(const std::string& savepath) : savepath{savepath}, gen{RandomGenerator()}, xy{std::make_pair(0, 0)} {
    db[xy] = Location(gen);
}

Map::Map() : gen{RandomGenerator()} {
}

Map::Map(const Map& other) : gen{RandomGenerator()}, savepath{other.savepath}, xy{other.xy}, db{other.db} {
}

Location Map::get() const noexcept {
    return db.at(xy);
}

Location& Map::getRef() noexcept {
    return db.at(xy);
}

Location Map::get(int x, int y) const {
    if (db.find(std::make_pair(x, y)) == db.end()) throw AdventureException("Map::get(x, y) location (x, y) not in map");
    return db.at(std::make_pair(x, y));
}

Location Map::get(Dir d) {
    std::pair<int, int> coords = getCoords(d);
    return db.at(coords);
}

Location Map::go(Dir d) {
    std::pair<int, int> coords = getCoords(d);
    xy = coords;
    return db.at(xy);
}

std::string Map::getPath() const noexcept {
    return savepath;
}

void Map::setPath(std::string newPath) noexcept {
    savepath = newPath;
}

void Map::save() const {
    fs::path p{savepath};
    if (!fs::exists(p)) fs::create_directory(p);

    std::ofstream o;
    o.open(savepath + "\\map.game");

    json out;
    out["x"] = xy.first;
    out["y"] = xy.second;
    out["db"] = json::array();

    std::for_each(db.begin(), db.end(), [&](const std::pair<std::pair<int,int>, Location>& pair){
        json item;
        item["x"] = pair.first.first;
        item["y"] = pair.first.second;
        item["biome"] = pair.second.biome;
        item["here"] = pair.second.save();
        out["db"].push_back(item);
    });

    o << out.dump(4) << std::endl;
    o.close();
}

Map* Map::load(const std::string& path) {
    fs::path p{path};
    if (!fs::exists(p)) throw AdventureException("Map::load() save does not exist.");

    std::ifstream i;
    i.open(path + "\\map.game");
    json filedata;
    i >> filedata;
    i.close();

    Map* m = new Map();
    m->savepath = path;
    m->xy = std::make_pair(filedata["x"].get<int>(), filedata["y"].get<int>());
    std::for_each(filedata["db"].begin(), filedata["db"].end(), [&](const json& item){
        std::pair<int, int> key = std::make_pair(item["x"].get<int>(), item["y"].get<int>());
        m->db[key] = Location(item["biome"].get<std::string>(), item["here"]);
    });

    return m;
}

std::pair<int, int> Map::getCoords(Dir d) {
    std::pair<int, int> coords;
    switch (d) {
        case Dir::NORTH:
            coords = std::make_pair(xy.first, xy.second + 1);
            break;
        case Dir::EAST:
            coords = std::make_pair(xy.first + 1, xy.second);
            break;
        case Dir::SOUTH:
            coords = std::make_pair(xy.first, xy.second - 1);
            break;
        case Dir::WEST:
            coords = std::make_pair(xy.first - 1, xy.second);
            break;
    }
    if (db.find(coords) == db.end()) db[coords] = Location(gen);
    return coords;
}