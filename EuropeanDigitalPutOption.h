#pragma once
#include "EuropeanDigitalOption.h"

class EuropeanDigitalPutOption : public EuropeanDigitalOption
{
public:
	EuropeanDigitalPutOption();
	std::string describe() override;
};
