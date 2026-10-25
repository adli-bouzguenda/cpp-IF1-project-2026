#include "CRRPricer.h"
#include <cmath>
#include <stdexcept>

CRRPricer::CRRPricer(Option* option, int depth, double asset_price, double up, double down, double interest_rate)
	: _option(option), _depth(depth), _asset_price(asset_price),
	  _up(up), _down(down), _interest_rate(interest_rate), _computed(false)
{
	if (option == nullptr)
		throw std::invalid_argument("Option cannot be null");

	// Asked in Part III: the CRR tree cannot price Asian options (path dependent)
	if (option->isAsianOption())
		throw std::invalid_argument("CRRPricer cannot price Asian options");

	if (depth < 0)
		throw std::invalid_argument("Depth must be nonnegative");
	if (asset_price <= 0)
		throw std::invalid_argument("Asset price must be positive");
	if (down <= -1)
		throw std::invalid_argument("D must be greater than -1");

	// No-arbitrage condition of the CRR model: D < R < U
	if (!(down < interest_rate && interest_rate < up))
		throw std::invalid_argument("Arbitrage opportunity: the model needs D < R < U");

	_tree.setDepth(depth);
}

double CRRPricer::stockPrice(int n, int i)
{
	return _asset_price * std::pow(1 + _up, i) * std::pow(1 + _down, n - i);
}

double CRRPricer::riskNeutralProbability()
{
	return (_interest_rate - _down) / (_up - _down);
}

/*
   CRR procedure (backward induction):
   1) at expiry N: H(N, i) = h(S(N, i)), the payoff, for every node i
   2) then from n = N - 1 down to 0:
      H(n, i) = [ q * H(n + 1, i + 1) + (1 - q) * H(n + 1, i) ] / (1 + R)
   i + 1 is the node reached after an up move, i the node reached after a down move.
*/
void CRRPricer::compute()
{
	double q = riskNeutralProbability();

	// 1) Payoffs at expiry
	for (int i = 0; i <= _depth; i++)
		_tree.setNode(_depth, i, _option->payoff(stockPrice(_depth, i)));

	// 2) Going back in time, step by step
	for (int n = _depth - 1; n >= 0; n--)
	{
		for (int i = 0; i <= n; i++)
		{
			double value = (q * _tree.getNode(n + 1, i + 1) + (1 - q) * _tree.getNode(n + 1, i))
				/ (1 + _interest_rate);
			_tree.setNode(n, i, value);
		}
	}
	_computed = true;
}

// The tree is filled only when needed, the first time a value is asked
double CRRPricer::get(int n, int i)
{
	if (!_computed)
		compute();
	return _tree.getNode(n, i);
}

/*
   Closed-form formula:
   H(0, 0) = 1 / (1 + R)^N * sum over i of  N! / (i! (N - i)!) * q^i * (1 - q)^(N - i) * h(S(N, i))

   For large N, N! overflows a double (170! is already too big).
   So each term is computed with logarithms: lgamma(x + 1) = ln(x!),
   and we take the exponential at the end.
*/
double CRRPricer::operator()(bool closed_form)
{
	if (!closed_form)
	{
		if (!_computed)
			compute();
		return _tree.getNode(0, 0);
	}

	double q = riskNeutralProbability();
	double sum = 0.0;

	for (int i = 0; i <= _depth; i++)
	{
		double log_weight = std::lgamma(_depth + 1.0) - std::lgamma(i + 1.0) - std::lgamma(_depth - i + 1.0)
			+ i * std::log(q) + (_depth - i) * std::log(1 - q);
		sum += std::exp(log_weight) * _option->payoff(stockPrice(_depth, i));
	}
	return sum / std::pow(1 + _interest_rate, _depth);
}
