// updated 12 June 2022

#ifndef GAMEENGINE_HPP
#define GAMEENGINE_HPP

#include <string>
#include <memory>
#include "Player.hpp"
#include "Map.hpp"
#include "Interface.hpp"
#include "GameData.hpp"

class GameEngine {
    public:
        GameEngine(std::shared_ptr<Interface> i, GameData data);
        GameEngine(const GameEngine& other) = delete;
        void save() const;
        void config();
        void run();

    private:
        std::shared_ptr<Player> player;
        std::shared_ptr<Map> map;
        std::shared_ptr<Interface> i;
};

#endif