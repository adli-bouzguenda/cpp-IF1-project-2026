#include "Option.h"
#include <stdexcept>

Option::Option(double expiry) : _expiry(expiry) {}

double Option::getExpiry()
{
	return _expiry;
}

double Option::payoffPath(const std::vector<double>& path)
{
	if (path.empty())
		throw std::invalid_argument("Price path cannot be empty");
	return payoff(path.back());   // European option: only the last price matters
}

// returns false for all non-asian options
bool Option::isAsianOption()
{
	return false;
}