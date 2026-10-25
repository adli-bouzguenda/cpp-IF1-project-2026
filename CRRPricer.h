#pragma once
#include "Option.h"
#include "BinaryTree.h"
#include <cmath>   // exp, sqrt, log... also available to the files that include this header

/*
   Prices an option with the Cox-Ross-Rubinstein (binomial) model.
   At each step the price goes up by (1 + U) or down by (1 + D),
   and the risk-free asset grows by (1 + R).
*/
class CRRPricer
{
public:
	// depth = N, asset_price = S0, up = U, down = D, interest_rate = R
	CRRPricer(Option* option, int depth, double asset_price, double up, double down, double interest_rate);

	// Fills the whole tree H(n, i) by backward induction (CRR procedure)
	void compute();

	// Returns H(n, i), the price of the option at step n and node i
	double get(int n, int i);

	// Price of the option today H(0, 0).
	// closed_form = true uses the closed-form formula instead of the tree.
	double operator()(bool closed_form = false);

private:
	Option* _option;
	int _depth;              // N
	double _asset_price;     // S0
	double _up;              // U
	double _down;            // D
	double _interest_rate;   // R

	BinaryTree<double> _tree;   // stores H(n, i)
	bool _computed;             // true once compute() has filled the tree

	// S(n, i) = S0 * (1 + U)^i * (1 + D)^(n - i)
	double stockPrice(int n, int i);

	// Risk-neutral probability q = (R - D) / (U - D)
	double riskNeutralProbability();
};
