// updated 30 April 2022

#ifndef MAP_HPP
#define MAP_HPP

#include <map>
#include <string>
#include <utility>
#include <vector>
#include "RandomGenerator.hpp"

enum class Dir {
    NORTH,
    EAST,
    SOUTH,
    WEST
};

struct Location {
    std::string biome;
    std::vector<std::string> here;

    Location();
    Location(RandomGenerator& gen);
    Location(const std::string& biome, const std::vector<std::string>& here);
};

class Map {
    public:
        // Load a map from the given save path
        // Save path should be the name of a save folder
        static Map load(const std::string& path);

    public:
        Map(const std::string& savepath);
        Map(const Map& other);
        Location get() const noexcept;
        Location get(int x, int y) const;
        Location get(Dir d);
        std::string getPath() const noexcept;
        void save() const;

    private:
        Map();

        std::map<std::pair<int, int>, Location> db;
        RandomGenerator gen;
        std::string savepath;
        std::pair<int, int> xy;
};

#endif