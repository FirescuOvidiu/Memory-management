#pragma once

const int numberAllocations = 100000;
const int fileLength = numberAllocations * 9 + numberAllocations * 5;


class GenerateTestUnits final
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

private:
	static const int numberObjectsAllocated;

	std::ofstream outputTU;
	std::ifstream inputTU;
	std::array<char, fileLength> buffer;
	std::pair<int, int> rangeObjectSize;
	std::pair<int, int> rangeNumberDeallocations;
};