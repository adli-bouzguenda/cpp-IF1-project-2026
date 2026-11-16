#pragma once
#include "AsianOption.h"

class AsianPutOption : public AsianOption {
public:
    // Constructor: initializes the observation dates and strike price
    AsianPutOption(const std::vector<double>& time_steps, double strike);
    // Computes the payoff of the Asian Put option
    double payoff(double spot) override;
private:
    // Strike price of the option
    double _strike;
};

