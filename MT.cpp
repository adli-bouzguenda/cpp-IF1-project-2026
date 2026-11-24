#include "MT.h"
using namespace std;
// Constructor: initializes the random generator
MT::MT() {
    random_device rd;
    _gen.seed(rd());
}
// Returns the unique instance of MT
MT& MT::instance() {
    static MT mt_instance;
    return mt_instance;
}
// Generates a random number uniformly distributed between 0 and 1
double MT::rand_unif() {
    std::uniform_real_distribution<double> dist(0, 1);

    return dist(instance()._gen);
}

// Generates a random number following a standard normal distribution
double MT::rand_norm() {
    std::normal_distribution<double> dist(0.0, 1.0);

    return dist(instance()._gen);
}
