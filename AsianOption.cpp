
#include "AsianOption.h"
#include <cstddef>
#include <stdexcept>

// Checks that the observation dates are not empty
// and returns the last date as the expiry date
static double check_time_steps(const std::vector<double>& time_steps)
{
    if (time_steps.empty()) {
        throw std::invalid_argument("Time steps cannot be empty");
    }
    // The dates must be positive and strictly increasing
    for (std::size_t i = 0; i < time_steps.size(); i++) {
        double previous = (i == 0) ? 0.0 : time_steps[i - 1];
        if (time_steps[i] <= previous) {
            throw std::invalid_argument("Time steps must be positive and increasing");
        }
    }
    return time_steps.back();
}

// Constructor: initializes the expiry and stores the observation dates
AsianOption::AsianOption(const std::vector<double>& time_steps)
    : Option(check_time_steps(time_steps))
{
    _time_steps = time_steps;
}

// Returns the observation dates of the Asian option
const std::vector<double>& AsianOption::getTimeSteps() const
{
    return _time_steps;
}

// Computes the payoff using the average asset price
double AsianOption::payoffPath(const std::vector<double>& spots)
{
    // Checks that the path contains at least one price
    if (spots.empty()) {
        throw std::invalid_argument("Price path cannot be empty");
    }

    double sum = 0;

    // Adds all the prices of the simulated path
    for (std::size_t i = 0; i < spots.size(); i++) {
        sum = sum + spots[i];
    }

    // Computes the arithmetic average
    double average = sum / spots.size();

    // Computes the payoff using the average price
    return payoff(average);
}

// Returns true because this is an Asian option
bool AsianOption::isAsianOption()
{
    return true;
}
