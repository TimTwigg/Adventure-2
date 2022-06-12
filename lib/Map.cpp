// updated 11 June 2022

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
        here.push_back("trees");
        if (biome == "Forest" || biome == "Jungle" || gen.getRandBool()) {
            int count = gen.getRandInt(0, 3);
            for (int i = 0; i < count; ++i) here.push_back("wood");
        }
    }

    if (biome == "Mountain" || (data["stone"].get<bool>() && gen.getRandBool())) {
        here.push_back("stone deposit");
        if (biome == "Mountain" || gen.getRandBool()) {
            int count = gen.getRandInt(0, 3);
            for (int i = 0; i < count; ++i) here.push_back("stone");
        }
    }

    if (data["metal"].get<bool>() && gen.getRandBool()) here.push_back("metal deposit");
    if (data["gold"].get<bool>() && gen.getRandBool()) here.push_back("gold deposit");
    if (biome != "River" && gen.getRandBool() && gen.getRandInt(0, 10) * data["river"].get<double>() >= gen.getRandInt(1, 10)) here.push_back("river");
    std::vector<std::string> civs = data["civ-types"].get<std::vector<std::string>>();
    if (civs.size() > 0 && gen.getRandBool() && gen.getRandBool()) here.push_back(civs[gen.getRandInt(0, civs.size()-1)]);
    std::vector<std::string> animals = data["animals"].get<std::vector<std::string>>();
    if (animals.size() > 0) {
        for (auto& a : animals) {
            if (gen.getRandBool()) here.push_back(a);
        }
    }
    // TODO: enemies
}

Location::Location(const std::string& biome, const std::vector<std::string>& here) : biome{biome}, here{here} {
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

Location Map::get(int x, int y) const {
    if (db.find(std::make_pair(x, y)) == db.end()) throw AdventureException("Map::get(x, y) location (x, y) not in map");
    return db.at(std::make_pair(x, y));
}

Location Map::get(Dir d) {
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
    return db.at(coords);
}

std::string Map::getPath() const noexcept {
    return savepath;
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
        item["here"] = pair.second.here;
        out["db"].push_back(item);
    });

    o << std::setw(4) << out << std::endl;
    o.close();
}

Map Map::load(const std::string& path) {
    fs::path p{"saves\\" + path};
    if (!fs::exists(p)) throw AdventureException("Map::load() save does not exist.");

    std::ifstream i;
    i.open("saves\\" + path + "\\map.game");
    json filedata;
    i >> filedata;
    i.close();

    Map m;
    m.savepath = path;
    m.xy = std::make_pair(filedata["x"].get<int>(), filedata["y"].get<int>());
    std::for_each(filedata["db"].begin(), filedata["db"].end(), [&](const json& item){
        std::pair<int, int> key = std::make_pair(item["x"].get<int>(), item["y"].get<int>());
        Location l = Location(item["biome"].get<std::string>(), item["here"].get<std::vector<std::string>>());
        m.db[key] = l;
    });

    return m;
}