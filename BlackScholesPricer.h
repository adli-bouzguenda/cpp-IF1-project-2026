#pragma once
#include "EuropeanVanillaOption.h"
#include "EuropeanDigitalOption.h"

/*
   Prices a European option with the Black-Scholes closed-form formulas.
   Two constructors: one for vanilla options (Part I), one for digital options (Part II).
   It is a friend of both option classes, so it can read the strike directly.
*/
class BlackScholesPricer
{
public:
	// Vanilla options (call / put)
	BlackScholesPricer(EuropeanVanillaOption* option, double asset_price, double interest_rate, double volatility);

	// Digital options (digital call / digital put)
	BlackScholesPricer(EuropeanDigitalOption* option, double asset_price, double interest_rate, double volatility);

	// Price of the option
	double operator()();

	// Delta of the option: sensitivity of the price to the underlying price S
	double delta();

private:
	// Only one of these two pointers is used, the other one stays nullptr
	EuropeanVanillaOption* _vanilla_option;
	EuropeanDigitalOption* _digital_option;

	double _asset_price;     // S, underlying price today
	double _interest_rate;   // r
	double _volatility;      // sigma

	// Strike, expiry and type of the option, whichever kind it is
	double strike();
	double expiry();
	bool isCall();

	// Payoff of the option, used when T = 0
	double payoffNow();

	// Cumulative distribution function N(x) and density n(x) of the standard normal law
	double normalCdf(double x);
	double normalPdf(double x);

	// d1 of the Black-Scholes formula (d2 = d1 - sigma * sqrt(T))
	double computeD1();
};
