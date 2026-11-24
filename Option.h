#pragma once
#include <string>

// Option = une option du menu de la Creperie du Projet C++
class Option
{
public:
	Option(const std::string& name, double price_euros);
	virtual ~Option() {}
	std::string getName();
	double getPrice();
	virtual std::string describe() = 0;
	virtual int calories();

private:
	std::string _name;
	double _price;
};
