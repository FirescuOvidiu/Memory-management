#pragma once


class GenerateTestUnits
{
public:
	GenerateTestUnits(int poolSize, int numberAllocations, int numberObjectsAllocated, std::pair<int, int> ObjectSize, std::pair<int, int> rangeNumberDeallocations) :
		poolSize(poolSize), numberAllocations(numberAllocations), numberObjectsAllocated(numberObjectsAllocated), rangeObjectSize(ObjectSize), rangeNumberDeallocations(rangeNumberDeallocations) {}

	void generateTU();
	void loadTU();

private:

	void deallocateObjects(const int countAllocations, std::set<int>& storeObjectId);

private:
	std::ofstream outputTU;
	std::ifstream inputTU;
	int poolSize;
	int numberAllocations;
	int numberObjectsAllocated;
	std::pair<int, int> rangeObjectSize;
	std::pair<int, int> rangeNumberDeallocations;
};