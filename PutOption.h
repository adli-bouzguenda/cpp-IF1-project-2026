#pragma once
#include "EuropeanVanillaOption.h"

// PutOption : la glace qu'on pose (put) sur la table... ou par terre
class PutOption : public EuropeanVanillaOption
{
public:
	PutOption(int scoops);
	std::string describe() override;
	int calories() override;
};
