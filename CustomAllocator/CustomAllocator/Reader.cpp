#include "stdafx.h"


std::list<PoolElement> Reader::readList(std::ifstream& input)
{
	std::list<PoolElement> list(readVariable<size_t>(input));

	// Parse the vector and read it's content from the input file
	for (auto& element : list)
	{
		input >> element;
	}
	
	return list;
}


std::set<PoolElement> Reader::readSet(std::ifstream& input)
{
	std::set<PoolElement> set;
	PoolElement var;
	const size_t size = readVariable<size_t>(input);

	for (int it = 0; it < (int)size; it++)
	{
		input >> var;
		set.insert(var);
	}

	return set;
}