#include "PutOption.h"

PutOption::PutOption(int scoops) : EuropeanVanillaOption("Glace 'Put'", 3.0, scoops)
{
}

std::string PutOption::describe()
{
	return "Le serveur pose la glace. Elle tombe. Strike ! Prix d'exercice : 0 euro.";
}

int PutOption::calories()
{
	return 0; // elle est par terre, donc ca ne compte pas
}
