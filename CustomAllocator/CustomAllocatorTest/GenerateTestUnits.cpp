#include "stdafx.h"


void GenerateTestUnits::generateTU()
{
	outputTU.open("generatedTU.txt", std::ofstream::out);

	outputTU << "\n\nPool size: " << poolSize << "\n";
	outputTU << "Number of allocations: " << numberAllocations << "\n";
	outputTU << "The size of objects allocated are between: " << rangeObjectSize.first << " and " << rangeObjectSize.second << "\n";
	outputTU << "Allocation format: Allocate objectId objectSize \n";
	outputTU << "Deallocation format: Deallocate objectId \n\n";

	std::random_device rd;
	std::uniform_int_distribution<int> distribution(rangeObjectSize.first, rangeObjectSize.second);
	// Need to add more distributions

	std::set<int> storeObjectId;
	int countAllocations = 0;

	for (int it = 0; it < numberObjectsAllocated && countAllocations < numberAllocations; it++)
	{
		outputTU << "Allocate " << it << " " << distribution(rd) << "\n";
		countAllocations++;
	}

	while (countAllocations < numberAllocations)
	{
		deallocateObjects(countAllocations, storeObjectId);

		for (const auto& itVisited : storeObjectId)
		{
			outputTU << "Allocate " << itVisited << " " << distribution(rd) << "\n";
			countAllocations++;
		}

		storeObjectId.clear();
	}

	for (int it = 0; it < numberObjectsAllocated; it++)
	{
		outputTU << "Deallocate " << it << "\n";
	}

	outputTU.close();
}


void GenerateTestUnits::loadTU()
{

}


void GenerateTestUnits::deallocateObjects(const int countAllocations, std::set<int>& storeObjectId)
{
	std::random_device rd;
	std::uniform_int_distribution<int> getNumberDealloc(rangeNumberDeallocations.first, rangeNumberDeallocations.second);
	std::uniform_int_distribution<int> getObjectId(0, numberObjectsAllocated - 1);
	int numberDeallocations = std::min(getNumberDealloc(rd), numberAllocations - countAllocations);
	int objectId = 0;

	for (int it = 0; it < numberDeallocations; it++)
	{
		do
		{
			objectId = getObjectId(rd);
		} while ((storeObjectId.find(objectId) != storeObjectId.end()));

		outputTU << "Deallocate " << objectId << "\n";
		storeObjectId.insert(objectId);
	}
}