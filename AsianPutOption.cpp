#include "AsianPutOption.h"
#include <stdexcept>
// Constructor
AsianPutOption::AsianPutOption(const std::vector<double>&time_steps, double strike)
    : AsianOption(time_steps)
{
    //Check if the strike is negative
    if (strike < 0)
        throw std::invalid_argument("Strike must be nonnegative");
    _strike = strike;
}
// payoff of an Asian Put option
double AsianPutOption::payoff(double spot)
{
    double payoff_value = 0;
    // The payoff is positive only if the average price is inferior to the strike
    if (spot < _strike)
    {
        payoff_value = _strike - spot;
    }
    // Returns the payoff 
    return payoff_value;
}
