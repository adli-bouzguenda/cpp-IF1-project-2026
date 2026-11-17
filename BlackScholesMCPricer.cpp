#include "BlackScholesMCPricer.h"
#include "AsianOption.h"
#include "MT.h"
#include <cmath>
#include <stdexcept>


//determinant of a 3x3 matrix
//input : m the matrix
//output : the determinant
static double det_3x3(double m[3][3])
{
	return m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1]) - m[0][1] * (m[1][0] * m[2][2] - m[1][2] * m[2][0]) + m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]);
}


//least squares regression of y on 1, x and x squared
//input : x and y the points of the regression
//output : vector with the 3 coefs
//the normal equations are solved with cramer rule
static std::vector<double> compute_regression(const std::vector<double>& x, const std::vector<double>& y)
{
	double a[3][3]={{0,0,0},{0,0,0},{0,0,0}}; //matrix of the normal equations
	double b[3]={0,0,0}; //right side
	//add each point to the normal equations
	for (int p=0; p < int(x.size()); p++)
	{
		double basis[3]={1.0,x[p],x[p] * x[p]}; //1, x and x squared
		//fill line j of the system
		for (int j=0; j < 3; j++)
		{
			b[j]+=basis[j] * y[p];
			//products of the basis
			for (int l=0; l < 3; l++)
				a[j][l]+=basis[j] * basis[l];
		}
	}
	double det=det_3x3(a);
	std::vector<double> coef(3);
	//cramer rule, column j is replaced by b
	for (int j=0; j < 3; j++)
	{
		double m[3][3];
		//copy a with b at column j
		for (int i=0; i < 3; i++)
		{
			//copy line i
			for (int l=0; l < 3; l++)
				m[i][l]=a[i][l];
			m[i][j]=b[i];
		}
		coef[j]=det_3x3(m) / det;
	}
	return coef;
}


//builds the pricer, no path at the start
//input : option, initial_price s0, interest_rate r, volatility sigma
//output : nothing
BlackScholesMCPricer::BlackScholesMCPricer(Option* option, double initial_price, double interest_rate, double volatility)
	: _option(option), _initial_price(initial_price), _interest_rate(interest_rate), _volatility(volatility),
	  _nb_paths(0), _current_estimate(0.0), _sum_squares(0.0), _pilot_done(false), _exercise_now(false)
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
//an american option uses the exercise rule of the bonus
void BlackScholesMCPricer::generate(int nb_paths)
{
	//a negative number of paths has no meaning
	if (nb_paths < 0)
		throw std::invalid_argument("The number of paths cannot be negative");
	//nothing to add
	if (nb_paths==0)
		return;
	std::vector<double> time_steps; //dates of the simulation
	//asian option uses its dates, the others just the expiry
	if (_option->isAsianOption())
		time_steps=dynamic_cast<AsianOption*>(_option)->getTimeSteps();
	else
		time_steps={_option->getExpiry()};
	double discount=std::exp(-_interest_rate * _option->getExpiry()); //exp(-rT)
	double new_sum=0.0; //sum of the new discounted payoffs
	//simulate the paths one after the other
	for (int p=0; p < nb_paths; p++)
	{
		double value; //discounted payoff of this path
		//bonus, american option with the exercise rule
		if (_option->isAmericanOption())
			value=american_discounted_payoff();
		else
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
			value=discount * _option->payoffPath(path);
		}
		new_sum+=value;
		_sum_squares+=value * value;
	}
	_current_estimate=(_current_estimate * _nb_paths + new_sum) / (_nb_paths + nb_paths); //update of the mean
	_nb_paths+=nb_paths;
}


//bonus, computes the exercise rule of the american option
//input : nothing, uses the params of the pricer
//output : nothing, _coefs and _exercise_now are filled
//pilot paths are separate from the paths of the price, so the rule does not see the future
//done only one time, the price is then the same whatever the size of the batches
void BlackScholesMCPricer::compute_pilot()
{
	int m=_nb_exercise_dates;
	double dt=_option->getExpiry() / m; //time between two dates
	double discount=std::exp(-_interest_rate * dt); //discount for one step
	std::vector<std::vector<double>> paths(_nb_pilot_paths, std::vector<double>(m + 1)); //pilot prices, needed to go backward
	//simulate the pilot paths
	for (int p=0; p < _nb_pilot_paths; p++)
	{
		paths[p][0]=_initial_price;
		//build the path step by step
		for (int k=1; k<=m; k++)
			paths[p][k]=paths[p][k - 1] * std::exp((_interest_rate - 0.5 * _volatility * _volatility) * dt + _volatility * std::sqrt(dt) * MT::rand_norm());
	}
	std::vector<double> cash_flow(_nb_pilot_paths); //value of each path at the current date
	//at expiry the payoff is received
	for (int p=0; p < _nb_pilot_paths; p++)
		cash_flow[p]=_option->payoff(paths[p][m]);
	_coefs.assign(m, std::vector<double>());
	//go back in time like in crr
	for (int k=m - 1; k>=1; k--)
	{
		std::vector<double> x, y; //points of the regression
		std::vector<int> in_money; //index of the paths in the money
		//discount one step and keep the paths in the money
		for (int p=0; p < _nb_pilot_paths; p++)
		{
			cash_flow[p]=cash_flow[p] * discount;
			//only these paths can be exercised
			if (_option->payoff(paths[p][k]) > 0)
			{
				x.push_back(paths[p][k] / _initial_price); //divided by s0 so the regression stays stable
				y.push_back(cash_flow[p]);
				in_money.push_back(p);
			}
		}
		//3 coefs so at least 3 points are needed, else no exercise at this date
		if (int(in_money.size()) < 3)
			continue;
		_coefs[k]=compute_regression(x, y);
		//exercise on the pilot paths where it is better
		for (int j=0; j < int(in_money.size()); j++)
		{
			double continuation=_coefs[k][0] + _coefs[k][1] * x[j] + _coefs[k][2] * x[j] * x[j]; //estimated continuation value
			double intrinsic=_option->payoff(paths[in_money[j]][k]);
			//same rule as in crr
			if (intrinsic>=continuation)
				cash_flow[in_money[j]]=intrinsic;
		}
	}
	double sum=0.0;
	//continuation value at time 0 is the mean of the pilot values
	for (int p=0; p < _nb_pilot_paths; p++)
		sum+=cash_flow[p] * discount;
	double payoff_now=_option->payoff(_initial_price);
	_exercise_now=(payoff_now > 0 && payoff_now>=sum / _nb_pilot_paths);
	_pilot_done=true;
}


//bonus, discounted payoff of one american path
//input : nothing
//output : payoff at the first date where the rule says to exercise, discounted to 0
//the path is simulated step by step and not stored
double BlackScholesMCPricer::american_discounted_payoff()
{
	//the rule is computed at the first call only
	if (!_pilot_done)
		compute_pilot();
	//better to exercise at time 0, same value for every path
	if (_exercise_now)
		return _option->payoff(_initial_price);
	int m=_nb_exercise_dates;
	double dt=_option->getExpiry() / m;
	double price=_initial_price;
	//move forward until the rule says to exercise
	for (int k=1; k < m; k++)
	{
		price=price * std::exp((_interest_rate - 0.5 * _volatility * _volatility) * dt + _volatility * std::sqrt(dt) * MT::rand_norm());
		double intrinsic=_option->payoff(price);
		//exercise is possible only in the money and if there are coefs at this date
		if (intrinsic > 0 && !_coefs[k].empty())
		{
			double x=price / _initial_price;
			double continuation=_coefs[k][0] + _coefs[k][1] * x + _coefs[k][2] * x * x;
			//exercise now if it is better than waiting
			if (intrinsic>=continuation)
				return intrinsic * std::exp(-_interest_rate * k * dt);
		}
	}
	price=price * std::exp((_interest_rate - 0.5 * _volatility * _volatility) * dt + _volatility * std::sqrt(dt) * MT::rand_norm()); //last step to the expiry
	return _option->payoff(price) * std::exp(-_interest_rate * _option->getExpiry());
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
	//rounding can give a tiny negative value when all the payoffs are equal
	if (variance < 0)
		variance=0.0;
	double half_width=1.96 * std::sqrt(variance / _nb_paths);
	return {mean - half_width,mean + half_width};
}
