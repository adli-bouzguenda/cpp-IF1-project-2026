
#pragma once
#include <vector>

/*
   Base class of every option of the project.
   It is abstract: payoff is pure virtual (= 0), because the payoff depends
   on the kind of option (call, put, ...). Each derived class writes its own.
*/
class Option
{
public:
    // Stores the expiry date T given as argument
    Option(double expiry);

    // Virtual destructor: needed in a base class, so that deleting a derived
    // object through an Option* correctly calls the derived destructor
    virtual ~Option() {}

    // Read access to the expiry
    double getExpiry();

    // The payoff function h(z) of the option: written by each derived class
    virtual double payoff(double z) = 0;

    // Computes the payoff from a simulated path of asset prices.
    // By default, the payoff depends only on the last price of the path.
    // Asian options will override this method to use the average price.
    virtual double payoffPath(const std::vector<double>& path);

    // Indicates whether the option is an Asian option.
    // Returns false by default and will be overridden in AsianOption.
    virtual bool isAsianOption();

protected:
    // protected (asked by the teacher): the derived classes can read _expiry directly
    double _expiry;   // expiry date T
};
