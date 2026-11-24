#include "AmericanOption.h"

AmericanOption::AmericanOption(const std::string& name, double price, int size_xl) : Option(name, price), _size(size_xl)
{
}

int AmericanOption::calories()
{
	return 900 * _size; // taille americaine
}

int AmericanOption::getSize()
{
	return _size;
}
