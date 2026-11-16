
#pragma once
#include <random>
/*
   Singleton random number generator.
   Uses a single Mersenne Twister generator
   for all Monte Carlo simulations.
*/
class MT {

public:
    // Generates a random number uniformly distributed between 0 and 1
    static double rand_unif();
    // Generates a random number following a standard normal distribution
    static double rand_norm();
private:
    MT();
    // Returns the unique instance of the random generator
    static MT& instance();
    // Mersenne Twister random number generator
    std::mt19937 _gen;
};
