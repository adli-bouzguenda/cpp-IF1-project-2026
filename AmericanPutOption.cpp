#include "AmericanPutOption.h"

AmericanPutOption::AmericanPutOption() : AmericanOption("Milkshake a emporter", 5.5, 2)
{
}

std::string AmericanPutOption::describe()
{
	return "Milkshake taille " + std::to_string(getSize()) + "XL. On peut l'exercer a tout moment : il est deja a moitie bu.";
}
