// updated 22 January 2022

#ifndef RANDOMGENERATOR_HPP
#define RANDOMGENERATOR_HPP

#include <random>
#include <vector>

class RandomGenerator {
    public:
        RandomGenerator();
        int getRandInt(int min, int max);
        std::vector<int> getRandIntVector(int min, int max, unsigned int len);
        std::vector<double> getRandDoubleVector(int min, int max, unsigned int len);
        bool getRandBool();
    
    private:
        std::random_device device;
        std::default_random_engine engine;
        std::uniform_int_distribution<int> distribution;
};

#endif