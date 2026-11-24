#include "CRRPricer.h"

CRRPricer::CRRPricer(const std::vector<Option*>& order) : _order(order)
{
}

int CRRPricer::operator()()
{
	int total = 0;
	for (Option* option : _order)
		total += option->calories();
	return total;
}

std::string CRRPricer::verdict()
{
	int total = (*this)();
	if (total < 1000)
		return "Repas raisonnable. Le prof de sport est fier.";
	if (total < 4000)
		return "Repas d'etudiant en periode de rendu de projet.";
	return "Arbitrage detecte : vous mangez plus que ce que vous depensez en energie.";
}
