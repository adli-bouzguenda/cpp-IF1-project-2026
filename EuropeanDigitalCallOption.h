#pragma once
#include "EuropeanDigitalOption.h"

class EuropeanDigitalCallOption : public EuropeanDigitalOption
{
public:
	EuropeanDigitalCallOption();
	std::string describe() override;
};
