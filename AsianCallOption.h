#pragma once
#include "AsianOption.h"

class AsianCallOption : public AsianOption
{
public:
	AsianCallOption();
	std::string describe() override;
};
