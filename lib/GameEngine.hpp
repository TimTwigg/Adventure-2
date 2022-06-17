// updated 16 June 2022

#ifndef GAMEENGINE_HPP
#define GAMEENGINE_HPP

#include <string>
#include <memory>
#include "Player.hpp"
#include "Map.hpp"
#include "Interface.hpp"
#include "GameData.hpp"
#include "RandomGenerator.hpp"
#include "json.hpp"
using json = nlohmann::json;

class GameEngine {
    public:
        static const std::vector<std::string> error_msgs;

    public:
        GameEngine(std::shared_ptr<Interface> i, GameData data);
        GameEngine(const GameEngine& other) = delete;
        void run();

    private:
        std::shared_ptr<Player> player;
        std::shared_ptr<Map> map;
        std::shared_ptr<Interface> i;
        json configs;
        // the query input by the player
        std::string query;
        // the words of the query split on whitespace
        std::vector<std::string> command;
        RandomGenerator gen;

        // helper functions

        // split a string on whitespace and return the words in a vector
        std::vector<std::string> split(const std::string& str);
        // print a Location (Location defined in Map.hpp)
        void printLocation(Location l);

        // command functions

        void look();
        void inventory();
        void go();
        void me();
        void ht();
        void hp();
        void attack();
        void mine();
        void chop();
        void eat();
        void drink();
        void take();
        void drop();
        void raid();
        void trade();
        void craft();
        void build();
        void recipe();
        void smoke();
        void sleep();
        void wait();
        void time();
        void fill();
        void train();
        void save();
        void help();
        void command_help();
        void object_help();
        void config();
};

#endif