#include "AsianCallOption.h"
#include <stdexcept>

// Constructor
AsianCallOption::AsianCallOption(const std::vector<double>& time_steps, double strike)
    : AsianOption(time_steps)
{
    // check if strike is negative
    if (strike < 0)
        throw std::invalid_argument("Strike must be nonnegative");
    _strike = strike;
}

// payoff of an Asian Call option
double AsianCallOption::payoff(double spot)
{
    double payoff_value = 0;
    // The payoff is positive only if the average price is superior to the strike
    if (spot > _strike)
    {
        payoff_value = spot - _strike;
    }
    // Returns the payoff 
    return payoff_value;
}
