#include "stdafx.h"


void Writer::writeList(std::ofstream& output, const std::list<PoolElement>& list)
{
	Writer::writeVariable(output, list.size());

	for (const auto& element : list)
	{
		output << element;
	}
}


void Writer::writeSet(std::ofstream& output, const std::set<PoolElement>& set)
{
	Writer::writeVariable(output, set.size());

	for (const auto& element : set)
	{
		output << element;
	}
}


void Writer::writeSortedLists(std::ofstream& output, const std::list<PoolElement>& first, const std::list<PoolElement>& second)
{
	auto itFirst = std::cbegin(first), itSecond = std::cbegin(second);

	if ((itFirst != std::cend(first)) && (itSecond != std::cend(second)))
	{
		if (itFirst->address > itSecond->address)
		{
			output << 0 << "\n";
		}
	}

	// We have two structures sorted by address and we need to write 
	// into a file the elements from the both structures sorted by address
	// We parse the structures simultaneous and compare the elements
	while (itFirst != std::cend(first) && itSecond != std::cend(second))
	{
		if (itFirst->address < itSecond->address)
		{
			output << itFirst->size << "\n";
			output << itSecond->size << "\n";
		}
		else
		{
			output << itSecond->size << "\n";
			output << itFirst->size << "\n";
		}

		itFirst++;
		itSecond++;
	}

	if (itFirst != std::cend(first))
	{
		output << itFirst->size << "\n";
	}
	if (itSecond != std::cend(second))
	{
		output << itSecond->size << "\n";
	}
}