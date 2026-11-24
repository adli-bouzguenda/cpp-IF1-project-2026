#include "AsianCallOption.h"

AsianCallOption::AsianCallOption() : AsianOption("Plateau maki-crepe", {"maki nutella", "maki banane", "maki caramel beurre sale"})
{
}

std::string AsianCallOption::describe()
{
	std::string text = "Plateau fusion : ";
	for (const std::string& piece : getPieces())
		text += piece + ", ";
	return text + "le chef ne s'excusera pas.";
}
