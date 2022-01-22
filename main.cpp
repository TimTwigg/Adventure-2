// Updated: 21 January 2022

#include <iostream>
#include "Player.hpp"
#include "SkillSets.hpp"
#include "json.hpp"
using json = nlohmann::json;

int main() {
    json set = Player::getSet(SkillSets::TRAVELER);
    std::cout << set.size() << std::endl;
    for (auto& [k, v] : set.items()) {
        std::cout << k << " " << v << std::endl;
    }
    //Player p{SkillSets::TRAVELER, "test"};
    //std::cout << "aasd" << std::endl;
    return 0;
}