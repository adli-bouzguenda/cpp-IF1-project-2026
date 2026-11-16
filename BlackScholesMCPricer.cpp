#include "BlackScholesMCPricer.h"
#include "AsianOption.h"
#include "MT.h"
#include <cmath>
#include <stdexcept>
using namespace std;

// Constructor: initializes the pricer and Monte Carlo statistics
BlackScholesMCPricer::BlackScholesMCPricer(
    Option* option, double spot, double rate, double volatility)
{
    if (option == nullptr) {
        throw invalid_argument("Option cannot be null");
    }

    _option = option;
    _spot = spot;
    _rate = rate;
    _volatility = volatility;
    _nb_paths = 0;
    _sum_payoff = 0.0;
    _sum_payoff_sq = 0.0;
}

// Generates n additional Monte Carlo paths
void BlackScholesMCPricer::generate(int n)
{
    if (n <= 0) {
        throw invalid_argument("Number of paths must be positive");
    }

    double T = _option->getExpiry();

    vector<double> time_steps;

    // Asian options use several observation dates
    AsianOption* asian_ptr = dynamic_cast<AsianOption*>(_option);

    if (asian_ptr != nullptr) {
        time_steps = asian_ptr->getTimeSteps();
    }
    else {
        // for European options which only need the final price
        time_steps.push_back(T);
    }

    // Simulates each stock price path
    for (int path = 0; path < n; ++path)
    {
        double spot_path = _spot;
        double previous_time = 0;

        vector<double> path_spots;

        for (size_t step = 0; step < time_steps.size(); ++step)
        {
            double dt = time_steps[step] - previous_time;

            if (dt < 0.0) {
                throw invalid_argument("Invalid observation dates");
            }

            // Generates a standard normal random variable
            double z = MT::rand_norm();

            // Simulates the next stock price using Black-Scholes
            spot_path *= exp(
                (_rate - 0.5 * _volatility * _volatility) * dt
                + _volatility * sqrt(dt) * z
            );
            // Stores the simulated price
            path_spots.push_back(spot_path);
            previous_time = time_steps[step];
        }

        // Computes the payoff of the simulated path
        double payoff_value = _option->payoffPath(path_spots);

        // Updates Monte Carlo statistics
        _sum_payoff += payoff_value;
        _sum_payoff_sq += payoff_value * payoff_value;
    }
    // Updates the total number of generated paths
    _nb_paths += n;
}
// Returns the Monte Carlo estimated option price
double BlackScholesMCPricer::operator()()
{
    if (_nb_paths == 0) {
        throw std::runtime_error("No paths generated");
    }
    double mean = _sum_payoff / double(_nb_paths);
    double T = _option->getExpiry();
    // Discounts the expected payoff to present value
    double discount = exp(-_rate * T);
    return discount * mean;
}

// Computes the 95% confidence interval
std::vector<double> BlackScholesMCPricer::confidenceInterval() const
{
    if (_nb_paths == 0) {
        throw runtime_error("No paths generated");
    }

    double mean = _sum_payoff / double(_nb_paths);

    double mean_sq = _sum_payoff_sq / double(_nb_paths);

    // Estimates the payoff variance
    double variance = mean_sq - mean * mean;

    if (variance < 0) {
        variance = 0;
    }

    double stddev = sqrt(variance);

    // Computes the 95% confidence interval margin
    double margin = 1.96 * stddev / sqrt(double(_nb_paths));

    double T = _option->getExpiry();
    double discount = exp(-_rate * T);

    double lower = discount * (mean - margin);
    double upper = discount * (mean + margin);

    return { lower, upper };
}

// Returns the total number of simulated paths
int BlackScholesMCPricer::getNbPaths() const
{
    return _nb_paths;
}
