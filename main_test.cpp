#include <iostream>
#include <cmath>
#include <vector>
#include <stdexcept>
#include "CallOption.h"
#include "PutOption.h"
#include "EuropeanDigitalCallOption.h"
#include "EuropeanDigitalPutOption.h"
#include "AsianCallOption.h"
#include "BlackScholesPricer.h"
#include "BinaryTree.h"
#include "CRRPricer.h"

/*
   Personal test file: it must NOT be in the zip sent to the teacher
   (the project says any main() has to be removed, the teacher uses his own).
*/
int main()
{
	try
	{
		double T = 1.0, K = 100.0, S = 100.0, r = 0.05, sigma = 0.2;

		// Part I: vanilla Black-Scholes
		CallOption call(T, K);
		PutOption put(T, K);
		BlackScholesPricer call_pricer(&call, S, r, sigma);
		BlackScholesPricer put_pricer(&put, S, r, sigma);
		std::cout << "Part I\n";
		std::cout << "Call price: " << call_pricer() << " (expected 10.4506)\n";
		std::cout << "Put price: " << put_pricer() << " (expected 5.5735)\n";

		// Part II: BinaryTree display
		std::cout << "\nPart II - BinaryTree<double> with depth 3\n";
		BinaryTree<double> tree;
		tree.setDepth(3);
		for (int n = 0; n <= 3; n++)
			for (int i = 0; i <= n; i++)
				tree.setNode(n, i, 10 * n + i);
		tree.display();

		// Part II: CRR, tree and closed form must give the same price
		std::cout << "Part II - CRR with S0 = 100, U = 0.05, D = -0.045, R = 0.01, N = 5\n";
		CRRPricer crr(&call, 5, 100.0, 0.05, -0.045, 0.01);
		std::cout << "Tree: " << crr() << ", closed form: " << crr(true) << "\n";
		std::cout << "H(5, 5) = " << crr.get(5, 5) << " (payoff at the top node)\n";

		// CRR with many steps converges to Black-Scholes
		int N = 1000;
		double h = T / N;
		double U = std::exp((r + 0.5 * sigma * sigma) * h + sigma * std::sqrt(h)) - 1;
		double D = std::exp((r + 0.5 * sigma * sigma) * h - sigma * std::sqrt(h)) - 1;
		double R = std::exp(r * h) - 1;
		CRRPricer crr_bs(&call, N, S, U, D, R);
		std::cout << "CRR with N = 1000: " << crr_bs() << " (Black-Scholes: " << call_pricer() << ")\n";
		std::cout << "Closed form with N = 1000: " << crr_bs(true) << "\n";

		// Part II: digital options
		EuropeanDigitalCallOption digital_call(T, K);
		EuropeanDigitalPutOption digital_put(T, K);
		BlackScholesPricer dc_pricer(&digital_call, S, r, sigma);
		BlackScholesPricer dp_pricer(&digital_put, S, r, sigma);
		std::cout << "\nDigital call: " << dc_pricer() << " (expected 0.5323), delta " << dc_pricer.delta() << "\n";
		std::cout << "Digital put: " << dp_pricer() << " (expected 0.4189), delta " << dp_pricer.delta() << "\n";
		// The two together always pay 1, so their sum is worth exp(-r T)
		std::cout << "Sum = " << dc_pricer() + dp_pricer() << ", exp(-rT) = " << std::exp(-r * T) << "\n";
		CRRPricer crr_digital(&digital_call, N, S, U, D, R);
		std::cout << "Digital call with CRR N = 1000: " << crr_digital() << "\n";

		// Errors that must be refused
		std::cout << "\nExceptions:\n";
		try { CRRPricer arbitrage(&call, 5, 100.0, 0.05, -0.045, 0.10); }
		catch (const std::invalid_argument& e) { std::cout << "- " << e.what() << "\n"; }

		std::vector<double> dates = { 0.25, 0.5, 0.75, 1.0 };
		AsianCallOption asian(dates, K);
		try { CRRPricer asian_crr(&asian, 5, 100.0, 0.05, -0.045, 0.01); }
		catch (const std::invalid_argument& e) { std::cout << "- " << e.what() << "\n"; }
	}
	catch (...)
	{
		std::cout << "Fatal Error" << std::endl;
		return -1;
	}
	return 0;
}
