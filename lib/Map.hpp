// updated 26 May 2024

#ifndef MAP_HPP
#define MAP_HPP

#include <map>
#include <string>
#include <utility>
#include <vector>
#include <memory>
#include "RandomGenerator.hpp"
#include "Thing.hpp"
#include "Enemy.hpp"
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
    // Create a new location
    Location(RandomGenerator& gen, std::vector<std::string> neighbors = {}, DangerLevel danger = DangerLevel::NORMAL, int level = 1);
    // Load a location from a json object
    Location(const std::string& biome, json here);
    json save() const;
    std::shared_ptr<Thing> accessThing(std::string name);
    void addThing(std::shared_ptr<Thing> thing);
    void removeThing(std::string thing);
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
        Location& getRef() noexcept;
        Location get(int x, int y) const;
        Location get(Dir d, int distance = 1);
        Location go(Dir d, DangerLevel danger = DangerLevel::NORMAL, int level = 1);
        std::string getPath() const noexcept;
        void setPath(std::string newPath) noexcept;
        void save() const;

    private:
        Map();
        std::vector<std::string> getNeighborBiomes(int x, int y);

        std::map<std::pair<int, int>, Location> db;
        RandomGenerator gen;
        std::string savepath;
        std::pair<int, int> xy;

        // get the appropriate coordinates from current location and direction
        // sets the location in those coords if not set
        std::pair<int, int> getCoords(Dir d, DangerLevel danger = DangerLevel::NORMAL, int level = 1);
        // unwrapped version of getCoords that takes a starting point
        std::pair<int, int> getCoords(Dir d, std::pair<int, int> start, DangerLevel danger = DangerLevel::NORMAL, int level = 1);
};

#endif