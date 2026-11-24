#include "AmericanCallOption.h"

AmericanCallOption::AmericanCallOption() : AmericanOption("Crepe burger triple cheddar", 12.9, 3)
{
}

std::string AmericanCallOption::describe()
{
	return "Crepe burger taille " + std::to_string(getSize()) + "XL. Exercice anticipe recommande : mangez-la avant qu'elle refroidisse.";
}
