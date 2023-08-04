// updated 4 August 2023

#include <gtest/gtest.h>
#include "Formulae.hpp"
#include "RandomGenerator.hpp"
#include <iostream>

TEST(formulaTests, animalDamage) {
    RandomGenerator gen;
    double baseDamage = 125;
    int diff = 1;
    // for (int i = 0; i < 10; ++i) {
    //     std::cout << Formula::animalDamage(baseDamage, gen, diff) << std::endl;
    // }
}

TEST(formulaTests, playerDamage) {
    RandomGenerator gen;
    double playerBaseDamage = 25;
    int diff = 1;
    // for (int i = 0; i < 10; ++i) {
    //     std::cout << Formula::playerDamage(playerBaseDamage, gen, diff) << std::endl;
    // }
}

TEST(formulaTests, enemyDamage) {
    RandomGenerator gen;
    double playerBaseDamage = 250;
    int diff = 1;
    // for (int i = 0; i < 10; ++i) {
    //     std::cout << Formula::enemyDamage(playerBaseDamage, gen, diff) << std::endl;
    // }
}

TEST(formulaTests, civDamage) {
    RandomGenerator gen;
    double playerBaseDamage = 500;
    int diff = 1;
    // for (int i = 0; i < 10; ++i) {
    //     std::cout << Formula::civilizationDamage(playerBaseDamage, gen, diff) << std::endl;
    // }
}