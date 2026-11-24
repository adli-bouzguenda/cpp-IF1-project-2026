#include "BlackScholesMCPricer.h"
#include "MT.h"

BlackScholesMCPricer::BlackScholesMCPricer(Option* dish) : _dish(dish), _nb_judges(0), _total_score(0.0)
{
}

void BlackScholesMCPricer::generate(int nb_judges)
{
	for (int j = 0; j < nb_judges; j++)
		_total_score += MT::humeur_du_chef();
	_nb_judges += nb_judges;
}

double BlackScholesMCPricer::operator()()
{
	if (_nb_judges == 0)
		return 0.0; // pas de jury, pas de note, pas de stress
	return _total_score / _nb_judges;
}

int BlackScholesMCPricer::getNbPaths()
{
	return _nb_judges;
}
