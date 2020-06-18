#pragma once

const int numberAllocations = 1000000;
const int fileLength = numberAllocations * 9 + numberAllocations * 5;


class GenerateTestUnits
{
public:
	GenerateTestUnits(std::pair<int, int> ObjectSize, std::pair<int, int> rangeNumberDeallocations) :
		rangeObjectSize(ObjectSize), rangeNumberDeallocations(rangeNumberDeallocations) {}

	void generateTU();
	void loadTU();
	void convertBinaryFile();

private:
	void deallocateRandomObjects(const int countAllocations, std::set<int>& storeObjectId);
	void readGeneratedTest();
	void initializeObjects(char* objects[]);

private:
	static const int numberObjectsAllocated;

	std::ofstream outputTU;
	std::ifstream inputTU;
	char buffer[fileLength];
	std::pair<int, int> rangeObjectSize;
	std::pair<int, int> rangeNumberDeallocations;
};