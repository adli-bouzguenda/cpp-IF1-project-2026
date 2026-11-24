#pragma once
#include "Option.h"

// La borne de commande digitale, qui ne comprend que 0 ou 1
class EuropeanDigitalOption : public Option
{
public:
	EuropeanDigitalOption(const std::string& name, double price);
	bool screenWorks();

protected:
	int _clicks;
};
