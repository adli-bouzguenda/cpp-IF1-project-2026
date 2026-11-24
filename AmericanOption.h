#pragma once
#include "Option.h"

// AmericanOption : tout ce qui peut etre mange AVANT la date d'expiration
class AmericanOption : public Option
{
public:
	AmericanOption(const std::string& name, double price, int size_xl);
	int calories() override;

protected:
	int getSize();

private:
	int _size;
};
