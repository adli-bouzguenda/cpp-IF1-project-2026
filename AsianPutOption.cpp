#include "AsianPutOption.h"

AsianPutOption::AsianPutOption() : AsianOption("Baguettes seules", {"baguette gauche", "baguette droite"})
{
}

std::string AsianPutOption::describe()
{
	return "Vous avez commande des baguettes pour manger une crepe. Respect.";
}
