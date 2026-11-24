#include "Option.h"

Option::Option(const std::string& name, double price_euros) : _name(name), _price(price_euros)
{
}

std::string Option::getName()
{
	return _name;
}

double Option::getPrice()
{
	return _price;
}

int Option::calories()
{
	return 250; // valeur moyenne d'une crepe, source : mamie
}
