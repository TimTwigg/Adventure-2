// updated 11 June 2022

#include <string>
#include <memory>
#include "GameEngine.hpp"
#include "Resource.hpp"
#include "CResource.hpp"
#include "Tool.hpp"
#include "Weapon.hpp"
#include "Animal.hpp"
#include "Enemy.hpp"
#include "Interface.hpp"

#include <iostream>

GameEngine::GameEngine(std::shared_ptr<Interface> interface, GameData data) : player{data.skillset, data.filepath}, map{player.getSavepath()}, interface{interface} {
}

GameEngine* GameEngine::load(std::shared_ptr<Interface> interface, const std::string& filepath) {
    Player p = Player::load(filepath);
    Map m = Map::load(filepath);
    return new GameEngine(p, m, interface);
}

GameEngine::GameEngine(Player& p, Map& m, std::shared_ptr<Interface> i) : player{p}, map{m}, interface{i} {
}

void GameEngine::save() const {
    player.save();
    map.save();
}

void GameEngine::run() {
    // mainloop
}