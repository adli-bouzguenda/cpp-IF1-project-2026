#include <iostream>
#include <vector>
#include "CallOption.h"
#include "PutOption.h"
#include "EuropeanDigitalCallOption.h"
#include "EuropeanDigitalPutOption.h"
#include "AsianCallOption.h"
#include "AsianPutOption.h"
#include "AmericanCallOption.h"
#include "AmericanPutOption.h"
#include "BinaryTree.h"
#include "BlackScholesPricer.h"
#include "CRRPricer.h"
#include "BlackScholesMCPricer.h"
#include "MT.h"

int main()
{
	std::cout << "=============================================\n";
	std::cout << "   BIENVENUE A LA CREPERIE DU PROJET C++ \n";
	std::cout << "   (le pricer d'options a ete vendu pour payer le loyer)\n";
	std::cout << "=============================================\n\n";

	CallOption call(2);
	PutOption put(1);
	EuropeanDigitalCallOption borne;
	EuropeanDigitalPutOption annulation;
	AsianCallOption maki;
	AsianPutOption baguettes;
	AmericanCallOption burger;
	AmericanPutOption milkshake;

	std::vector<Option*> order = {&call, &put, &borne, &annulation, &maki, &baguettes, &burger, &milkshake};

	std::cout << "--- La commande de l'equipe ---\n";
	for (Option* option : order)
		std::cout << " * " << option->getName() << " (" << option->getPrice() << " euros)\n   -> " << option->describe() << "\n";

	BlackScholesPricer caisse(15.0);
	for (Option* option : order)
		caisse.add(option);
	std::cout << "\n--- Addition chez Black & Scholes ---\n";
	std::cout << " Total avec pourboire et cafe impose : " << caisse() << " euros\n";
	std::cout << " Delta : " << caisse.delta() << " (personne ne sait ce que c'est)\n";

	CRRPricer compteur(order);
	std::cout << "\n--- Compteur de Repas Ridicules ---\n";
	std::cout << " Calories : " << compteur() << " kcal\n";
	std::cout << " Verdict : " << compteur.verdict() << "\n";

	std::cout << "\n--- Plonge ---\n";
	BinaryTree<std::string> pile;
	pile.push("assiette propre");
	pile.push("assiette sale");
	pile.push("assiette de quelqu'un qui a dit 'je la lave demain'");
	pile.display();

	std::cout << "\n--- MasterChef, jury Monte Carlo ---\n";
	BlackScholesMCPricer jury(&burger);
	jury.generate(1000);
	std::cout << " Note moyenne du burger sur " << jury.getNbPaths() << " juges : " << jury() << "/10\n";

	std::cout << "\n--- Machine a Tiramisu ---\n";
	for (int day = 1; day <= 5; day++)
		std::cout << " Jour " << day << " : " << MT::dessert_du_jour() << "\n";

	std::cout << "\nMerci de votre visite. Le vrai projet est sur la branche main, promis.\n";
	return 0;
}
