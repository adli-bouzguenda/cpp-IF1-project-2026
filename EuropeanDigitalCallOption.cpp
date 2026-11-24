#include "EuropeanDigitalCallOption.h"

EuropeanDigitalCallOption::EuropeanDigitalCallOption() : EuropeanDigitalOption("Crepe commandee sur borne", 1.0)
{
}

std::string EuropeanDigitalCallOption::describe()
{
	int tries = 1;
	while (!screenWorks())
		tries++;
	return "Commande validee apres " + std::to_string(tries) + " essais. Payoff : 1 crepe, ou 0. C'est digital.";
}
