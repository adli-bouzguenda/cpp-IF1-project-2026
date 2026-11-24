#pragma once
#include "Option.h"
#include <vector>
/*
   Base class for Asian options.
   Unlike European options, Asian options use the average
   asset price to calculate their payoff.
*/
class AsianOption : public Option
{
public:
    // Constructor: initializes the option with its observation dates
    explicit AsianOption(const std::vector<double>& time_steps);
    // Returns the observation dates of the Asian option
    const std::vector<double>& getTimeSteps() const;
    // Computes the payoff using the average price of the path
    double payoffPath(const std::vector<double>& spots) final;
    // Returns true because the option is Asian
    bool isAsianOption() override;
private:
    // Stores the dates at which the asset price is observed
    std::vector<double> _time_steps;
};
