#include "AsianOption.h"

AsianOption::AsianOption(const std::string& name, const std::vector<std::string>& pieces) : Option(name, 2.0 * pieces.size()), _pieces(pieces)
{
}

std::vector<std::string> AsianOption::getPieces()
{
	return _pieces;
}

int AsianOption::calories()
{
	return 45 * static_cast<int>(_pieces.size()); // moyenne sur le chemin, evidemment
}
