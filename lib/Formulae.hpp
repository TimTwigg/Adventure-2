// updated 18 June 2022

#ifndef FORMULAE_HPP
#define FORMULAE_HPP

#include "RandomGenerator.hpp"

namespace Formula {
    double animalDamage(double baseDamage, RandomGenerator& gen, int diff_ratio = 1);
    double playerDamage(double baseDamage, RandomGenerator& gen, int diff_ratio = 1);
    double enemyDamage(double baseDamage, RandomGenerator& gen, int diff_ratio = 1);
    double civilizationDamage(double baseDamage, RandomGenerator& gen, int diff_ratio = 1);
}

#endif