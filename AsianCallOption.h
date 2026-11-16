#pragma once
#include "AsianOption.h"
/*
   For an asian call option, the payoff
   depends on the average asset price
   and the strike price.
*/
class AsianCallOption : public AsianOption {
public:
    // Constructor: initializes the observation dates and strike price
    AsianCallOption(const std::vector<double>& time_steps, double strike);

    // Computes the payoff of the Asian Call option
    double payoff(double spot) override;

private:
    // Strike price of the option
    double _strike;
};
