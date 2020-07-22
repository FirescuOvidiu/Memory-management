#include "stdafx.h"


void Writer::writeList(std::ofstream& output, std::list<PoolElement> list)
{
	Writer::writeVariable(output, list.size());

	for (const auto& element : list)
	{
		output << element;
	}
}


void Writer::writeSet(std::ofstream& output, std::set<PoolElement> set)
{
	Writer::writeVariable(output, set.size());

	for (const auto& element : set)
	{
		output << element;
	}
}