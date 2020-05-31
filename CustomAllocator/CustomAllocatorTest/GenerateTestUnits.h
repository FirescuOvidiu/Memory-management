#pragma once

class GenerateTestUnits
{
public:
	GenerateTestUnits(int poolSize, int numberAllocations, std::pair<int, int> ObjectSize, std::pair<int, int> rangeNumberDeallocations) :
		poolSize(poolSize), numberAllocations(numberAllocations), rangeObjectSize(ObjectSize), rangeNumberDeallocations(rangeNumberDeallocations) {}

	void generateTU();
	void loadTU();
	void convertBinaryFile();

private:
	void deallocateObjects(const int countAllocations, std::set<int>& storeObjectId);

private:
	static const int numberObjectsAllocated;
	int numberAllocations;

	std::ofstream outputTU;
	std::ifstream inputTU;
	int poolSize;
	std::pair<int, int> rangeObjectSize;
	std::pair<int, int> rangeNumberDeallocations;
};