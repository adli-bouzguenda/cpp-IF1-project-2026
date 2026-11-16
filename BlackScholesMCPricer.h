#pragma once

#include "Option.h"
#include <vector>

class BlackScholesMCPricer {
public:
    // Constructor: initializes the Monte Carlo pricer
    BlackScholesMCPricer(Option* option, double spot, double rate, double volatility);
    // Generates n additional simulated stock price paths
    void generate(int n);
    // Returns the estimated option price
    double operator()();
    // Returns the 95% confidence interval
    std::vector<double> confidenceInterval() const;
    // Returns the total number of simulated paths
    int getNbPaths() const;

private:
    Option* _option;
    double _spot;
    double _rate;
    double _volatility;

    // Number of generated paths
    int _nb_paths;

    // Sum of payoffs and squared payoffs
    double _sum_payoff;
    double _sum_payoff_sq;
};


