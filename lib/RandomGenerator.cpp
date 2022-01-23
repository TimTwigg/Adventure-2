// updated 22 January 2022

#include <random>
#include <vector>
#include "RandomGenerator.hpp"
#include "AdventureException.hpp"

RandomGenerator::RandomGenerator() : device{}, engine{device()} {
}

int RandomGenerator::getRandInt(int min, int max) {
    if (min > max) throw AdventureException("RandomGenerator::getRandInt invalid parameters");
    distribution = std::uniform_int_distribution<int>(min, max);
    return distribution(engine);
}

std::vector<int> RandomGenerator::getRandIntVector(int min, int max, unsigned int len) {
    if (min > max) throw AdventureException("RandomGenerator::getRandIntVector invalid parameters");
    distribution = std::uniform_int_distribution<int>(min, max);
    std::vector<int> v;
    for (int i = 0; i < len; ++i) {
        v.push_back(distribution(engine));
    }
    return v;
}

std::vector<double> RandomGenerator::getRandDoubleVector(int min, int max, unsigned int len) {
    if (min > max) throw AdventureException("RandomGenerator::getRandDoubleVector invalid parameters");
    distribution = std::uniform_int_distribution<int>(min, max);
    std::vector<double> v;
    for (int i = 0; i < len; ++i) {
        v.push_back(distribution(engine) / 10.0);
    }
    return v;
}

bool RandomGenerator::getRandBool() {
    distribution = std::uniform_int_distribution<int>(0, 1);
    return distribution(engine);
}