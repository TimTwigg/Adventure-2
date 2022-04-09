// updated 9 April 2022

#ifndef MAP_HPP
#define MAP_HPP

#include <map>
#include <string>
#include <utility>
#include <vector>
#include "RandomGenerator.hpp"

enum class Dir {
    UP,
    RIGHT,
    DOWN,
    LEFT
};

struct Location {
    std::string biome;
    std::vector<std::string> here;

    Location(RandomGenerator& gen);
    Location(const std::string& biome, const std::vector<std::string>& here);
};

class Map {
    public:
        static Map load(const std::string& path);

    public:
        Map();
        Location get();
        Location get(int x, int y);
        Location get(Dir d);
        void save() const;

    private:
        Location generate();

        std::map<std::pair<int, int>, Location> db;
        RandomGenerator gen;
};

#endif