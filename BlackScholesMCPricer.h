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

	//bonus american option with the longstaff schwartz method
	static const int _nb_exercise_dates=50; //the american option is approx by 50 exercise dates
	static const int _nb_pilot_paths=100000; //paths used only once to compute the regression
	bool _pilot_done; //true when the regression coefs are known
	bool _exercise_now; //true if exercising at time 0 is better
	std::vector<std::vector<double>> _coefs; //3 coefs for each date, empty if no exercise at this date
	void compute_pilot(); //regression on the pilot paths, done one time
	double american_discounted_payoff(); //simulates one path with the exercise rule
};
