#include "CallOption.h"

CallOption::CallOption(int scoops) : EuropeanVanillaOption("Glace 'Call'", 3.5, scoops)
{
}

std::string CallOption::describe()
{
	return "Vous appelez le serveur. Il ne vient pas. Vous rappelez. Il arrive avec " + std::to_string(getScoops()) + " boule(s) de vanille.";
}
