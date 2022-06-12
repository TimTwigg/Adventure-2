// updated 11 June 2022

#ifndef GAMEENGINE_HPP
#define GAMEENGINE_HPP

#include <string>
#include <memory>
#include "Player.hpp"
#include "Map.hpp"
#include "Interface.hpp"
#include "SkillSets.hpp"

struct GameData {
    SkillSets skillset;
    std::string filepath;
};

class GameEngine {
    public:
        static GameEngine* load(std::shared_ptr<Interface> interface, const std::string& filepath);

    public:
        GameEngine(std::shared_ptr<Interface> interface, GameData data);
        GameEngine(const GameEngine& other) = delete;
        ~GameEngine() = default;
        void save() const;
        void run();

    private:
        GameEngine(Player& p, Map& m, std::shared_ptr<Interface> i);

        Player player;
        Map map;
        std::shared_ptr<Interface> interface;
};

#endif