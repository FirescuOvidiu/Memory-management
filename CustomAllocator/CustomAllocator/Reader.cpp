#include "stdafx.h"


std::list<PoolElement> Reader::readList(std::ifstream& input)
{
	std::list<PoolElement> list(readVariable<size_t>(input));

	for (auto& element : list)
	{
		input >> element;
	}
	
	return list;
}


std::set<PoolElement> Reader::readSet(std::ifstream& input)
{
	std::set<PoolElement> set;
	PoolElement element;
	const int size = (int)readVariable<size_t>(input);

	for (int it = 0; it < size; it++)
	{
		input >> element;
		set.insert(element);
	}

	return set;
}


std::vector<std::set<PoolElement>> Reader::readVectorOfSets(std::ifstream& input)
{
	std::vector<std::set<PoolElement>> vectorOfSets(readVariable<size_t>(input));

	for (auto& set : vectorOfSets)
	{
		set = readSet(input);
	}

	return vectorOfSets;
}
