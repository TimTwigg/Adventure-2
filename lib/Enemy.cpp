// updated 9 August 2023

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
#include  "json.hpp"
using json = nlohmann::json;

Enemy::Enemy(std::string name) {
    strHelp::format(name);
    if (name.size() < 1) throw AdventureException("Enemy: enemy name required");
    json data = FileReader::getFromFile("enemies.json", name);
    this->name = name;
    this->type = ENT::ENEMY;
    this->value = data["value"].get<int>();
    this->maxHP = data["health"].get<int>();
    this->hp = maxHP;
    this->damage = data["damage"].get<int>();
    this->xp = data["xp"].get<int>();
    for (const std::pair<std::string, int>& item : data["drops"].get<std::map<std::string, int>>()) {
        if (item.first.substr(0, 7) == "weapons") this->drops.push_back(std::shared_ptr<Object>(new Weapon(item.first.substr(8))));
        else if (item.first.substr(0, 9) == "resources") this->drops.push_back(std::shared_ptr<Object>(new Resource(item.first.substr(10), item.second)));
        else if (item.first.substr(0, 5) == "tools") this->drops.push_back(std::shared_ptr<Object>(Tool::fromString(item.first.substr(6))));
    }
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
    for (const auto& obj : drops) {
        OBJCLASS type = obj->getType();
        if (gen.getRandBool()) {
            if (type == OBJCLASS::RESOURCE) {
                Resource* r = static_cast<Resource*>(obj.get());
                int num = gen.getRandInt(1, r->getCount());
                v.push_back(std::shared_ptr<Object>(new Resource(r->getName(), num)));
            }
            else {
                if (gen.getRandBool()) v.push_back(std::shared_ptr<Object>(obj));
            }
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