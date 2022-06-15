// updated 15 June 2022

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
        std::string query;
        RandomGenerator gen;

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
        void clear();
        void save();
        void help();
        void command_help();
        void object_help();
        void config();
};

#endif