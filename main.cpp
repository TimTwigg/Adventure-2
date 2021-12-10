// Updated: 8 December 2021

#include <iostream>
#include <vector>
#include <map>
#include "lib\\Player.hpp"
#include "lib\\SkillSets.hpp"

int main() {
    Player player{SkillSets::WARRIOR, "test"};
    player.save();
    std::cout << "done" << std::endl;

    return 0;
}