#include "BlackScholesPricer.h"

BlackScholesPricer::BlackScholesPricer(double tip_percent) : _tip(tip_percent)
{
}

void BlackScholesPricer::add(Option* option)
{
	_order.push_back(option);
}

double BlackScholesPricer::operator()()
{
	double total = 0.0;
	for (Option* option : _order)
		total += option->getPrice();
	return total * (1.0 + _tip / 100.0) + 1.20; // + 1,20 euros : le cafe noir de Black, impose
}

double BlackScholesPricer::delta()
{
	return 0.01; // sensibilite de l'addition a la mauvaise humeur de Scholes
}
