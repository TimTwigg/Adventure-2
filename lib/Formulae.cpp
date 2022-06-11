// updated 11 June 2022

#include "Formulae.hpp"
#include "RandomGenerator.hpp"

double Formula::animalDamage(double baseDamage, RandomGenerator& gen, int diff_ratio) {
    int ratio = 5;
    int modifier = gen.getRandInt(2, 6);
    int bonus = gen.getRandInt(1, 6) * ratio;
    return ((baseDamage * modifier) / ratio) * diff_ratio + bonus;
}

double Formula::playerDamage(double baseDamage, RandomGenerator& gen, int diff_ratio) {
    int ratio = 5;
    int modifier = gen.getRandInt(1, 6);
    int bonus = gen.getRandInt(1, 6) * ratio;
    return (baseDamage * modifier) / diff_ratio + bonus;
}

double Formula::enemyDamage(double baseDamage, RandomGenerator& gen, int diff_ratio) {
    int ratio = 5;
    int modifier = gen.getRandInt(3, 6);
    int bonus = gen.getRandInt(1, 6) * ratio;
    return ((baseDamage * modifier) / ratio) * diff_ratio + bonus;
}