#pragma once
#include "EuropeanVanillaOption.h"

// CallOption : la glace qu'on commande en appelant le serveur trois fois
class CallOption : public EuropeanVanillaOption
{
public:
	CallOption(int scoops);
	std::string describe() override;
};
