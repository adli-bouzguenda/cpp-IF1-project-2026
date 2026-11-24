#include "EuropeanVanillaOption.h"

EuropeanVanillaOption::EuropeanVanillaOption(const std::string& name, double price, int scoops) : Option(name, price), _scoops(scoops)
{
}

int EuropeanVanillaOption::getScoops()
{
	return _scoops;
}

int EuropeanVanillaOption::calories()
{
	return 140 * _scoops; // chaque boule compte, meme la derniere
}
