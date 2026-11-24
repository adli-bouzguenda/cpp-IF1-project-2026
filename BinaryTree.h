#pragma once
#include <vector>
#include <iostream>

// BinaryTree : en fait c'est une pile d'assiettes. Binaire : propre (1) ou sale (0).
template <typename T>
class BinaryTree
{
public:
	void push(T plate)
	{
		_plates.push_back(plate);
	}

	T pop()
	{
		T top = _plates.back();
		_plates.pop_back();
		return top;
	}

	int size()
	{
		return static_cast<int>(_plates.size());
	}

	void display()
	{
		std::cout << "  Pile d'assiettes (du haut vers le bas) :\n";
		for (int i = size() - 1; i >= 0; i--)
			std::cout << "   [ " << _plates[i] << " ]\n";
		std::cout << "  ~~~~~~~~~~~ (le plan de travail)\n";
	}

private:
	std::vector<T> _plates;
};
