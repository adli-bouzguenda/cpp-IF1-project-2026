#pragma once
#include "Option.h"

// Une boule de glace vanille, servie a l'europeenne (avec une gaufrette)
class EuropeanVanillaOption : public Option
{
public:
	EuropeanVanillaOption(const std::string& name, double price, int scoops);
	int getScoops();
	int calories() override;

private:
	int _scoops;
};
