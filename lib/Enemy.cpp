// updated 9 June 2022

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <utility>
#include <sstream>
#include "Object.hpp"
#include "Entity.hpp"
#include "Enemy.hpp"
#include "Formulae.hpp"

Enemy::Enemy(std::string name) {

}

unsigned int Enemy::attack() noexcept {
    return 0;
}

bool Enemy::attack(double dmg) noexcept {
    return false;
}

std::vector<std::shared_ptr<Object>> Enemy::getDrops() noexcept {
    return std::vector<std::shared_ptr<Object>>();
}

Enemy* Enemy::fromString(const std::string& code) {
    return new Enemy("");
}