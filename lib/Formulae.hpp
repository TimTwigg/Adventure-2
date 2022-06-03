// updated 1 June 2022

#ifndef FORMULAE_HPP
#define FORMULAE_HPP

#include "RandomGenerator.hpp"

namespace Formula {
    double animalDamage(double baseDamage, RandomGenerator& gen);
    double playerDamage(double baseDamage, RandomGenerator& gen);
    double enemyDamage(double baseDamage, RandomGenerator& gen);
}

#endif