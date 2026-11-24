#pragma once
#include <string>

// MT = Machine a Tiramisu. Singleton, parce qu'il n'y en a qu'une et qu'elle est toujours en panne.
class MT
{
public:
	static std::string dessert_du_jour();
	static int humeur_du_chef();

private:
	MT();
	MT(const MT&) = delete;
	MT& operator=(const MT&) = delete;
	static MT& instance();
	unsigned int _state;
	unsigned int next();
};
