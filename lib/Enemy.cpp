// updated 26 May 2024

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
#include "AdventureException.hpp"
#include "FileReader.hpp"
#include "Resource.hpp"
#include "Weapon.hpp"
#include "Tool.hpp"
#include "StringHelpers.hpp"
#include "Factory.hpp"
#include  "json.hpp"
using json = nlohmann::json;

Enemy::Enemy(std::string name) {
    strHelp::format(name);
    if (name.size() < 1) throw AdventureException("Enemy: enemy name required");
    json data = FileReader::getFromFile("enemies.json", name);
    this->name = name;
    this->type = ENT::ENEMY;
    this->value = 0;
    this->maxHP = data["health"].get<int>();
    this->hp = maxHP;
    this->damage = data["damage"].get<int>();
    this->xp = data["xp"].get<int>();
    this->minimumLevel = data["minimumLevel"].get<int>();
    this->minimumDanger = data["minimumDanger"].get<DangerLevel>();
    this->drops = data["drops"].get<std::map<std::string, int>>();
}

double Enemy::attack() noexcept {
    return Formula::enemyDamage(damage, gen);
}

bool Enemy::attack(double dmg) noexcept {
    hp -= dmg;
    if (hp <= 0) {
        hp = 0;
        return true;
    }
    return false;
}

std::vector<std::shared_ptr<Object>> Enemy::getDrops() noexcept {
    std::vector<std::shared_ptr<Object>> v;
    Factory f;
    for (const std::pair<std::string, int>& item : this->drops) {
        if (item.first == "wealth") this->value = gen.getRandInt(1, 8) / 4.0 * item.second;
        else if (gen.getRandBool()) {
            std::shared_ptr<Object> p = std::shared_ptr<Object>(static_cast<Object*>(f.make(item.first)));
            if ((p->getType() == OBJCLASS::RESOURCE || p->getType() == OBJCLASS::CRESOURCE) && item.second > 1) {
                Resource* r = static_cast<Resource*>(p.get());
                r->add(item.second-1);
            }
            v.push_back(p);
        }
    }
    return v;
}

Enemy* Enemy::fromString(const std::string& code) {
    std::stringstream ss{code};
    std::vector<std::string> v;
    while (ss.good()) {
        std::string s;
        std::getline(ss, s, ',');
        v.push_back(s);
    }
    Enemy* e = new Enemy(v[1]);
    e->hp = std::stof(v[2]);
    return e;
}

int Enemy::getMinimumLevel() const noexcept {
    return minimumLevel;
}

DangerLevel Enemy::getMinimumDanger() const noexcept {
    return minimumDanger;
}