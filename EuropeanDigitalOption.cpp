#include "EuropeanDigitalOption.h"

EuropeanDigitalOption::EuropeanDigitalOption(const std::string& name, double price) : Option(name, price), _clicks(0)
{
}

bool EuropeanDigitalOption::screenWorks()
{
	_clicks++;
	return _clicks % 3 == 0; // la borne marche une fois sur trois, comme toutes les bornes
}
