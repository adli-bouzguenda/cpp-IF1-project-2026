#pragma once
#include "AmericanOption.h"

class AmericanPutOption : public AmericanOption
{
public:
	AmericanPutOption();
	std::string describe() override;
};
