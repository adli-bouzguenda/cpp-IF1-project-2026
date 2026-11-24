#include "MT.h"

MT::MT() : _state(2026)
{
}

MT& MT::instance()
{
	static MT machine;
	return machine;
}

unsigned int MT::next()
{
	_state = _state * 1103515245u + 12345u; // generateur fait maison, approuve par personne
	return (_state / 65536u) % 32768u;
}

std::string MT::dessert_du_jour()
{
	const std::string desserts[] = {"tiramisu", "tiramisu", "tiramisu", "tiramisu (encore)", "machine en panne, revenez demain"};
	return desserts[instance().next() % 5];
}

int MT::humeur_du_chef()
{
	return static_cast<int>(instance().next() % 11); // sur 10, mais parfois 0
}
