#pragma once
#include "Option.h"
#include <vector>

class BlackScholesMCPricer
{
public:
	BlackScholesMCPricer(Option* option, double initial_price, double interest_rate, double volatility);
	int getNbPaths(); //number of paths generated until now
	void generate(int nb_paths); //simulates new paths and updates the estimate
	double operator()(); //current estimate of the price
	std::vector<double> confidenceInterval(); //95 percent interval [lower,upper]

private:
	Option* _option;
	double _initial_price; //s0
	double _interest_rate; //r
	double _volatility; //sigma
	int _nb_paths; //paths already used for the estimate
	double _current_estimate; //mean of the discounted payoffs
	double _sum_squares; //sum of the squared discounted payoffs, for the interval

	/*
	   Bonus: American options with Monte Carlo (Longstaff-Schwartz).
	   The American option can be exercised at any time, it is approximated
	   by an option that can be exercised at AMERICAN_EXERCISE_DATES dates
	   equally spaced between 0 and T (a Bermudan option).
	*/
	static const int AMERICAN_EXERCISE_DATES = 50;

	//simulates nb_paths paths and returns the discounted payoff of each one,
	//using the Longstaff-Schwartz exercise rule
	std::vector<double> americanDiscountedPayoffs(int nb_paths);
};
