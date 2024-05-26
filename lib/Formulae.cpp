// updated 26 May 2024

#include "Formulae.hpp"
#include "RandomGenerator.hpp"
#include "Defaults.hpp"

double Formula::animalDamage(double baseDamage, RandomGenerator& gen, int diff_ratio) {
    double modifier = gen.getRandInt(5, 15) / 10.0;
    int bonus = gen.getRandInt(1, 6) * 5;
    return baseDamage * modifier * diff_ratio + bonus;
}

double Formula::playerDamage(double baseDamage, RandomGenerator& gen, int diff_ratio) {
    double modifier = gen.getRandInt(5, 15) / 10.0;
    int bonus = gen.getRandInt(1, 6) * 2;
    return (baseDamage * modifier) / diff_ratio + bonus;
}

double Formula::enemyDamage(double baseDamage, RandomGenerator& gen, int diff_ratio) {
    double modifier = gen.getRandInt(10, 18) / 10.0;
    int bonus = gen.getRandInt(1, 6) * 3;
    return baseDamage * modifier * diff_ratio + bonus;
}

double Formula::civilizationDamage(double baseDamage, RandomGenerator& gen, int diff_ratio) {
    double modifier = gen.getRandInt(8, 18) / 10.0;
    int bonus = gen.getRandInt(2, 6) * 5;
    return baseDamage * modifier * diff_ratio + bonus;
}

int Formula::xpToLevelUp(int currentLevel) {
    return (currentLevel + 1) * DEFAULTS::xp_per_level;
}