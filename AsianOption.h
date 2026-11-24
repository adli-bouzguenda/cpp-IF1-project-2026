#pragma once
#include "Option.h"
#include <vector>

// AsianOption : le plateau de sushis (oui, dans une creperie, on innove)
class AsianOption : public Option
{
public:
	AsianOption(const std::string& name, const std::vector<std::string>& pieces);
	std::vector<std::string> getPieces();
	int calories() override;

private:
	std::vector<std::string> _pieces;
};
