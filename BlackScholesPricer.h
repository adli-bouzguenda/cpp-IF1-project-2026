#pragma once
#include "Option.h"
#include <vector>

// Black & Scholes : les deux caissiers de la creperie. Black sert le cafe, Scholes rend la monnaie.
class BlackScholesPricer
{
public:
	BlackScholesPricer(double tip_percent);
	void add(Option* option);
	double operator()();
	double delta();

private:
	std::vector<Option*> _order;
	double _tip;
};
