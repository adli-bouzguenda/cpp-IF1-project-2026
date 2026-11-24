#pragma once
#include "Option.h"
#include "BinaryTree.h"
#include <vector>

// CRR = Compteur de Repas Ridicules : additionne les calories de la commande
class CRRPricer
{
public:
	CRRPricer(const std::vector<Option*>& order);
	int operator()();
	std::string verdict();

private:
	std::vector<Option*> _order;
};
