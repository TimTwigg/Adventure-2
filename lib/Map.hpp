// updated 19 June 2022

#ifndef MAP_HPP
#define MAP_HPP

#include <map>
#include <string>
#include <utility>
#include <vector>
#include <memory>
#include "RandomGenerator.hpp"
#include "Thing.hpp"
#include "json.hpp"
using json = nlohmann::json;

enum class Dir {
    NORTH,
    EAST,
    SOUTH,
    WEST
};

struct Location {
    std::string biome;
    std::vector<std::string> miscHere;
    std::vector<std::shared_ptr<Thing>> thingsHere;

    Location();
    Location(RandomGenerator& gen);
    Location(const std::string& biome, json here);
    json save() const;
};

class Map {
    public:
        // Load a map from the given save path
        // Save path should be the name of a save folder
        static Map* load(const std::string& path);

    public:
        Map(const std::string& savepath);
        Map(const Map& other);
        Location get() const noexcept;
        Location get(int x, int y) const;
        Location get(Dir d);
        Location go(Dir d);
        std::string getPath() const noexcept;
        void setPath(std::string newPath) noexcept;
        void save() const;

    private:
        Map();

        std::map<std::pair<int, int>, Location> db;
        RandomGenerator gen;
        std::string savepath;
        std::pair<int, int> xy;

        // get the appropriate coordinates from current location and direction
        // sets the location in those coords if not set
        std::pair<int, int> getCoords(Dir d);
};

#endif