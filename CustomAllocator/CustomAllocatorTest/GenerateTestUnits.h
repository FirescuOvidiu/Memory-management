#pragma once

const int numberAllocations = 10000;
const int fileLength = numberAllocations * 9 + numberAllocations * 5;


class GenerateTestUnits
{
public:
	GenerateTestUnits(int poolSize, std::pair<int, int> ObjectSize, std::pair<int, int> rangeNumberDeallocations) :
		poolSize(poolSize), rangeObjectSize(ObjectSize), rangeNumberDeallocations(rangeNumberDeallocations) {}

	void generateTU();
	void loadTU();
	void convertBinaryFile();

private:
	void deallocateRandomObjects(const int countAllocations, std::set<int>& storeObjectId);
	void readGeneratedTest();

private:
	static const int numberObjectsAllocated;
	static const char allocationNotation, deallocationNotation;

	std::ofstream outputTU;
	std::ifstream inputTU;
	char buffer[fileLength];
	int poolSize;
	std::pair<int, int> rangeObjectSize;
	std::pair<int, int> rangeNumberDeallocations;
};