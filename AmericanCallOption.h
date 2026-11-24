#pragma once
#include "AmericanOption.h"

class AmericanCallOption : public AmericanOption
{
public:
	AmericanCallOption();
	std::string describe() override;
};
