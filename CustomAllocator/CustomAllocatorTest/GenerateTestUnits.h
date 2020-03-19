#pragma once


class GenerateTestUnits
{
public:


private:
	std::ofstream outputTU;
	std::ifstream inputTU;
	int poolSize;
	int numberObjectsAllocated;
	std::pair<int, int> ObjectSize;
	std::pair<int, int> numberDeallocations;
};