#pragma once
#include "AsianOption.h"

class AsianPutOption : public AsianOption
{
public:
	AsianPutOption();
	std::string describe() override;
};
