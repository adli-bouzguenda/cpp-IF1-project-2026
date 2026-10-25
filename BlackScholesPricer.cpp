#include "BlackScholesPricer.h"
#include <cmath>
#include <stdexcept>

BlackScholesPricer::BlackScholesPricer(EuropeanVanillaOption* option, double asset_price, double interest_rate, double volatility)
	: _vanilla_option(option), _digital_option(nullptr),
	  _asset_price(asset_price), _interest_rate(interest_rate), _volatility(volatility)
{
	if (option == nullptr)
		throw std::invalid_argument("Option cannot be null");
}

BlackScholesPricer::BlackScholesPricer(EuropeanDigitalOption* option, double asset_price, double interest_rate, double volatility)
	: _vanilla_option(nullptr), _digital_option(option),
	  _asset_price(asset_price), _interest_rate(interest_rate), _volatility(volatility)
{
	if (option == nullptr)
		throw std::invalid_argument("Option cannot be null");
}

// _strike is readable because BlackScholesPricer is a friend of both classes
double BlackScholesPricer::strike()
{
	if (_digital_option != nullptr)
		return _digital_option->_strike;
	return _vanilla_option->_strike;
}

double BlackScholesPricer::expiry()
{
	if (_digital_option != nullptr)
		return _digital_option->getExpiry();
	return _vanilla_option->getExpiry();
}

bool BlackScholesPricer::isCall()
{
	if (_digital_option != nullptr)
		return _digital_option->GetOptionType() == EuropeanDigitalOption::optionType::call;
	return _vanilla_option->GetOptionType() == EuropeanVanillaOption::optionType::call;
}

double BlackScholesPricer::payoffNow()
{
	if (_digital_option != nullptr)
		return _digital_option->payoff(_asset_price);
	return _vanilla_option->payoff(_asset_price);
}

/*
   N(x) = P(Z <= x) for Z ~ N(0, 1).
   The standard library gives erfc (complementary error function), and
   N(x) = 0.5 * erfc(-x / sqrt(2)). This is the hint of the project.
*/
double BlackScholesPricer::normalCdf(double x)
{
	return 0.5 * std::erfc(-x / std::sqrt(2.0));
}

// n(x) = exp(-x^2 / 2) / sqrt(2 pi), the derivative of N(x)
double BlackScholesPricer::normalPdf(double x)
{
	const double pi = std::acos(-1.0);
	return std::exp(-0.5 * x * x) / std::sqrt(2.0 * pi);
}

// d1 = [ ln(S / K) + (r + sigma^2 / 2) * T ] / (sigma * sqrt(T))
double BlackScholesPricer::computeD1()
{
	double S = _asset_price;
	double K = strike();
	double T = expiry();
	double r = _interest_rate;
	double sigma = _volatility;

	return (std::log(S / K) + (r + 0.5 * sigma * sigma) * T) / (sigma * std::sqrt(T));
}

/*
   Vanilla options:
   Call = S * N(d1) - K * exp(-r T) * N(d2)
   Put  = K * exp(-r T) * N(-d2) - S * N(-d1)

   Digital options (they pay 1, so only the probability term remains):
   Digital call = exp(-r T) * N(d2)
   Digital put  = exp(-r T) * N(-d2)

   with d2 = d1 - sigma * sqrt(T)
*/
double BlackScholesPricer::operator()()
{
	double S = _asset_price;
	double K = strike();
	double T = expiry();
	double r = _interest_rate;

	// At expiry (T = 0) the formula divides by 0: the price is just the payoff
	if (T == 0)
		return payoffNow();

	double d1 = computeD1();
	double d2 = d1 - _volatility * std::sqrt(T);
	double discount = std::exp(-r * T);   // value today of 1 paid at T

	if (_digital_option != nullptr)
	{
		if (isCall())
			return discount * normalCdf(d2);
		return discount * normalCdf(-d2);
	}

	if (isCall())
		return S * normalCdf(d1) - K * discount * normalCdf(d2);
	return K * discount * normalCdf(-d2) - S * normalCdf(-d1);
}

/*
   Delta = derivative of the price with respect to S
   Vanilla call: N(d1)                 Vanilla put: N(d1) - 1
   Digital call: exp(-r T) * n(d2) / (S * sigma * sqrt(T))
   Digital put:  minus the same value
*/
double BlackScholesPricer::delta()
{
	double S = _asset_price;
	double K = strike();
	double T = expiry();

	// At expiry: vanilla delta is 1 (or -1) in the money, 0 otherwise.
	// A digital payoff is a step, its delta is 0 everywhere except exactly at K.
	if (T == 0)
	{
		if (_digital_option != nullptr)
			return 0.0;
		if (isCall())
			return (S > K) ? 1.0 : 0.0;
		return (S < K) ? -1.0 : 0.0;
	}

	double d1 = computeD1();

	if (_digital_option != nullptr)
	{
		double d2 = d1 - _volatility * std::sqrt(T);
		double value = std::exp(-_interest_rate * T) * normalPdf(d2) / (S * _volatility * std::sqrt(T));
		if (isCall())
			return value;
		return -value;
	}

	if (isCall())
		return normalCdf(d1);
	return normalCdf(d1) - 1.0;
}
