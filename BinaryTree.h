#pragma once
#include <vector>
#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>

/*
   Data structure of the CRR tree: level n (from 0 to N) has n + 1 nodes.
   It is a template class: T is the type stored in each node
   (double for the option prices, bool later for the American exercise policy).

   Everything is written in this .h file and there is no BinaryTree.cpp:
   the compiler needs the full code of a template wherever it is used,
   so a template cannot be split between a .h and a .cpp like a normal class.
*/
template <typename T>
class BinaryTree
{
public:
	// Empty tree at the start, setDepth gives it its size
	BinaryTree() : _depth(0) {}

	/*
	   Sets N and resizes _tree: N + 1 levels, level n holding n + 1 values.
	   resize allocates or frees the memory by itself, no new / delete needed.
	*/
	void setDepth(int depth)
	{
		if (depth < 0)
			throw std::invalid_argument("Depth must be nonnegative");
		_depth = depth;
		_tree.resize(_depth + 1);
		for (int n = 0; n <= _depth; n++)
			_tree[n].resize(n + 1);
	}

	// Stores value at level n, node i. at() throws an exception if (n, i) is outside the tree
	void setNode(int n, int i, T value)
	{
		_tree.at(n).at(i) = value;
	}

	// Returns the value stored at level n, node i
	T getNode(int n, int i)
	{
		return _tree.at(n).at(i);
	}

	/*
	   Prints all the values twice:
	   1) one line per level (simple list)
	   2) as a centered triangle with branches.
	   For 2), we first measure the longest value once printed (for example 3
	   characters for 695), then every value gets a cell of that size plus a
	   margin, and is centered in it. Each level is shifted by half a cell,
	   so that a node sits exactly between its two children ("lego" method).
	*/
	void display()
	{
		// 1) One line per level
		for (int n = 0; n <= _depth; n++)
		{
			for (int i = 0; i <= n; i++)
				std::cout << _tree[n][i] << " ";
			std::cout << "\n";
		}
		std::cout << "\n";

		// 2a) Longest value once converted to text
		std::size_t max_length = 1;
		for (int n = 0; n <= _depth; n++)
			for (int i = 0; i <= n; i++)
				if (toText(_tree[n][i]).size() > max_length)
					max_length = toText(_tree[n][i]).size();

		// 2b) Cell width: longest value + 2 spaces of margin, made even so that half a cell is exact
		int cell = int(max_length) + 2;
		if (cell % 2 == 1)
			cell++;
		int total_width = (_depth + 1) * cell;

		for (int n = 0; n <= _depth; n++)
		{
			// Line of values: node i of level n is centered at column x
			std::string line(total_width, ' ');
			for (int i = 0; i <= n; i++)
			{
				int x = (_depth - n) * cell / 2 + i * cell + cell / 2;
				std::string text = toText(_tree[n][i]);
				int begin = x - int(text.size()) / 2;
				line.replace(begin, text.size(), text);
			}
			std::cout << line << "\n";

			// Line of branches: the children of a node are at x - cell/2 and x + cell/2,
			// so "/" and "\\" are drawn halfway, at x - cell/4 and x + cell/4
			if (n < _depth)
			{
				std::string branches(total_width, ' ');
				for (int i = 0; i <= n; i++)
				{
					int x = (_depth - n) * cell / 2 + i * cell + cell / 2;
					branches[x - cell / 4] = '/';
					branches[x + cell / 4] = '\\';
				}
				std::cout << branches << "\n";
			}
		}
		std::cout << "\n";
	}

private:
	int _depth;                          // N, the last level of the tree
	std::vector<std::vector<T>> _tree;   // _tree[n][i] = value at level n, node i

	// Converts a value to the text that std::cout would print (used to measure and center it)
	std::string toText(T value)
	{
		std::ostringstream stream;
		stream << value;
		return stream.str();
	}
};
