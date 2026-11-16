#include "BlackScholesMCPricer.h"
#include "AsianOption.h"
#include "MT.h"
#include <cmath>
#include <stdexcept>
#include <utility>


//builds the pricer, no path at the start
//input : option, initial_price s0, interest_rate r, volatility sigma
//output : nothing
BlackScholesMCPricer::BlackScholesMCPricer(Option* option, double initial_price, double interest_rate, double volatility)
	: _option(option), _initial_price(initial_price), _interest_rate(interest_rate), _volatility(volatility),
	  _nb_paths(0), _current_estimate(0.0), _sum_squares(0.0)
{
}


//gives the number of paths
//input : nothing
//output : paths generated since the beginning
int BlackScholesMCPricer::getNbPaths()
{
	return _nb_paths;
}


//generates nb_paths new paths and updates the estimate
//input : nb_paths the number of new paths
//output : nothing, the estimate is changed
//for a non asian option the only date is the expiry so m=1
//an american option is sent to the longstaff-schwartz method below (bonus)
//the paths are not kept, only the sums
void BlackScholesMCPricer::generate(int nb_paths)
{
	if (nb_paths < 0)
		throw std::invalid_argument("The number of paths cannot be negative");
	if (nb_paths == 0)
		return; //nothing to add
	double new_sum=0.0; //sum of the new discounted payoffs

	//bonus: american option, priced with the longstaff-schwartz method
	if (_option->isAmericanOption())
	{
		std::vector<double> values=americanDiscountedPayoffs(nb_paths);
		for (int p=0; p < nb_paths; p++)
		{
			new_sum+=values[p];
			_sum_squares+=values[p] * values[p];
		}
		_current_estimate=(_current_estimate * _nb_paths + new_sum) / (_nb_paths + nb_paths);
		_nb_paths+=nb_paths;
		return;
	}

	std::vector<double> time_steps; //dates of the simulation
	//asian option uses its dates, the others just the expiry
	if (_option->isAsianOption())
		time_steps=dynamic_cast<AsianOption*>(_option)->getTimeSteps();
	else
		time_steps={_option->getExpiry()};
	double discount=std::exp(-_interest_rate * _option->getExpiry()); //exp(-rT)
	//simulate the paths one after the other
	for (int p=0; p < nb_paths; p++)
	{
		std::vector<double> path(time_steps.size()); //local, not stored
		double price=_initial_price;
		double previous_time=0.0;
		//build the path date by date
		for (int k=0; k < int(time_steps.size()); k++)
		{
			double dt=time_steps[k] - previous_time; //tk minus tk-1
			price=price * std::exp((_interest_rate - 0.5 * _volatility * _volatility) * dt + _volatility * std::sqrt(dt) * MT::rand_norm());
			path[k]=price;
			previous_time=time_steps[k];
		}
		double value=discount * _option->payoffPath(path); //discounted payoff of this path
		new_sum+=value;
		_sum_squares+=value * value;
	}
	_current_estimate=(_current_estimate * _nb_paths + new_sum) / (_nb_paths + nb_paths); //update of the mean
	_nb_paths+=nb_paths;
}


/*
   Bonus: Longstaff-Schwartz algorithm for an American option.

   1) Simulate nb_paths paths on the dates t_k = k * T / M, k = 1..M.
   2) At maturity, each path gets the payoff h(S(t_M)), exercised at t_M.
   3) Going back in time, at each date t_k:
      - keep only the paths that are in the money (h(S) > 0), the only ones
        where exercising can make sense
      - regress the discounted future cash flow of those paths on the basis
        1, x, x^2 with x = S / S0 (least squares): this gives an estimate
        C(S) of the continuation value, i.e. the value of NOT exercising now
      - if h(S) >= C(S), exercise now: the cash flow of the path becomes
        h(S) received at t_k
   4) Each path is worth its cash flow discounted from its exercise date to 0.

   The regression uses the paths of the batch, so the batch must be large
   (several thousands of paths) for the exercise rule to be accurate.
   Exercise at t = 0 is not considered, which is fine as long as the option
   is not deep in the money today.

   input : nb_paths the number of paths of the batch
   output : vector with the discounted payoff of each path
*/
std::vector<double> BlackScholesMCPricer::americanDiscountedPayoffs(int nb_paths)
{
	const int M=AMERICAN_EXERCISE_DATES;
	double dt=_option->getExpiry() / M;
	double drift=(_interest_rate - 0.5 * _volatility * _volatility) * dt;
	double diffusion=_volatility * std::sqrt(dt);
	double one_step_discount=std::exp(-_interest_rate * dt);

	//prices[k][p] = price of path p at date t_k (k = 0 is today)
	std::vector<std::vector<double>> prices(M + 1, std::vector<double>(nb_paths, _initial_price));
	for (int p=0; p < nb_paths; p++)
		for (int k=1; k <= M; k++)
			prices[k][p]=prices[k - 1][p] * std::exp(drift + diffusion * MT::rand_norm());

	//cash flow of each path and the date index where it is received
	std::vector<double> cash_flow(nb_paths);
	std::vector<int> exercise_date(nb_paths, M);
	for (int p=0; p < nb_paths; p++)
		cash_flow[p]=_option->payoff(prices[M][p]);

	//backward induction on the exercise dates
	for (int k=M - 1; k >= 1; k--)
	{
		//normal equations of the regression: A * beta = b, A is 3x3
		double A[3][3]={{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
		double b[3]={0, 0, 0};
		int nb_in_the_money=0;
		for (int p=0; p < nb_paths; p++)
		{
			if (_option->payoff(prices[k][p]) <= 0)
				continue;
			double x=prices[k][p] / _initial_price;
			double basis[3]={1.0, x, x * x};
			double y=cash_flow[p] * std::pow(one_step_discount, exercise_date[p] - k); //future cash flow brought back to t_k
			for (int i=0; i < 3; i++)
			{
				b[i]+=basis[i] * y;
				for (int j=0; j < 3; j++)
					A[i][j]+=basis[i] * basis[j];
			}
			nb_in_the_money++;
		}
		//not enough points for a regression with 3 coefficients: no exercise at this date
		if (nb_in_the_money < 3)
			continue;

		//solve A * beta = b by gaussian elimination with partial pivoting
		bool singular=false;
		for (int col=0; col < 3 && !singular; col++)
		{
			int pivot=col;
			for (int row=col + 1; row < 3; row++)
				if (std::fabs(A[row][col]) > std::fabs(A[pivot][col]))
					pivot=row;
			if (std::fabs(A[pivot][col]) < 1e-12)
			{
				singular=true;
				break;
			}
			for (int j=0; j < 3; j++)
				std::swap(A[col][j], A[pivot][j]);
			std::swap(b[col], b[pivot]);
			for (int row=col + 1; row < 3; row++)
			{
				double factor=A[row][col] / A[col][col];
				for (int j=col; j < 3; j++)
					A[row][j]-=factor * A[col][j];
				b[row]-=factor * b[col];
			}
		}
		if (singular)
			continue;
		double beta[3];
		for (int i=2; i >= 0; i--)
		{
			beta[i]=b[i];
			for (int j=i + 1; j < 3; j++)
				beta[i]-=A[i][j] * beta[j];
			beta[i]/=A[i][i];
		}

		//exercise where the payoff now is at least the estimated continuation value
		for (int p=0; p < nb_paths; p++)
		{
			double exercise_value=_option->payoff(prices[k][p]);
			if (exercise_value <= 0)
				continue;
			double x=prices[k][p] / _initial_price;
			double continuation=beta[0] + beta[1] * x + beta[2] * x * x;
			if (exercise_value >= continuation)
			{
				cash_flow[p]=exercise_value;
				exercise_date[p]=k;
			}
		}
	}

	//discount each cash flow from its exercise date to today
	std::vector<double> values(nb_paths);
	for (int p=0; p < nb_paths; p++)
		values[p]=cash_flow[p] * std::pow(one_step_discount, exercise_date[p]);
	return values;
}


//current price of the option
//input : nothing
//output : the estimate
//throws an exception if no path was generated
double BlackScholesMCPricer::operator()()
{
	//no path means no estimate
	if (_nb_paths==0)
		throw std::runtime_error("Price undefined, call generate first");
	return _current_estimate;
}


//95 percent confidence interval of the price
//input : nothing
//output : vector with the lower bound then the upper bound
//1.96 is the quantile of the normal law
std::vector<double> BlackScholesMCPricer::confidenceInterval()
{
	double mean=(*this)(); //throws too if there is no path
	double variance=_sum_squares / _nb_paths - mean * mean;
	double half_width=1.96 * std::sqrt(variance / _nb_paths);
	return {mean - half_width,mean + half_width};
}
