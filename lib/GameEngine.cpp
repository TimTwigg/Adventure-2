// updated 14 June 2022

#include <string>
#include <memory>
#include <cstdlib>
#include "GameEngine.hpp"
//#include "Resource.hpp"
//#include "CResource.hpp"
//#include "Tool.hpp"
//#include "Weapon.hpp"
//#include "Animal.hpp"
//#include "Enemy.hpp"
#include "Interface.hpp"
#include "TextArt.hpp"

GameEngine::GameEngine(std::shared_ptr<Interface> i, GameData data) : i{i} {
    if (data.isNew) {
        player = std::shared_ptr<Player>(new Player(data.skillset, data.filename));
        map = std::shared_ptr<Map>(new Map(player->getSavepath()));
    }
    else {
        player = std::shared_ptr<Player>(Player::load(data.filename));
        map = std::shared_ptr<Map>(Map::load(data.filename));
    }
}

void GameEngine::save() const {
    player->save();
    map->save();
}

void GameEngine::config() {

}

void GameEngine::run() {
    // mainloop
    save();
}