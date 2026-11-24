#pragma once
#include "Option.h"
#include <vector>

// MC = MasterChef : le jury Black & Scholes note les plats au hasard (methode Monte Carlo, evidemment)
class BlackScholesMCPricer
{
public:
	BlackScholesMCPricer(Option* dish);
	void generate(int nb_judges);
	double operator()();
	int getNbPaths();

private:
	Option* _dish;
	int _nb_judges;
	double _total_score;
};
