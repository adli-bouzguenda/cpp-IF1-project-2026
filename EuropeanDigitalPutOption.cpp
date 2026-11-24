#include "EuropeanDigitalPutOption.h"

EuropeanDigitalPutOption::EuropeanDigitalPutOption() : EuropeanDigitalOption("Annulation sur borne", 0.0)
{
}

std::string EuropeanDigitalPutOption::describe()
{
	return "Vous essayez d'annuler. La borne affiche : 'Erreur 404 : bouton annuler introuvable'.";
}
