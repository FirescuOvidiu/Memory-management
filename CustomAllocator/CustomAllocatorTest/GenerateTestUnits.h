#pragma once


class GenerateTestUnits
{
public:
	GenerateTestUnits(int poolSize, int numberObjectsAllocated, std::pair<int, int> ObjectSize, std::pair<int, int> numberDeallocations) :
		poolSize(poolSize), numberObjectsAllocated(numberObjectsAllocated), ObjectSize(ObjectSize), numberDeallocations(numberDeallocations) {}

	void generateTU();
	void loadTU();

private:
	std::ofstream outputTU;
	std::ifstream inputTU;
	int poolSize;
	int numberObjectsAllocated;
	std::pair<int, int> ObjectSize;
	std::pair<int, int> numberDeallocations;
};