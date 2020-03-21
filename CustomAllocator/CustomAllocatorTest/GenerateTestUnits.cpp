#include "stdafx.h"

/*
	Method used to generate test units based on a distribution
	How we generate the test units:
		- allocate X objects at the start (X is decided by us)
		- while we haven't made a total of Z allocations: (Z is decided by us)
			- deallocate Y random objects (Y is random)
			- allocate   Y random objects
		- deallocate all objects
*/
void GenerateTestUnits::generateTU()
{
	outputTU.open("generatedTU.txt", std::ofstream::out);

	// Useful informations about the TU generated
	outputTU << "\n\nPool size: " << poolSize << "\n";
	outputTU << "Number of allocations: " << numberAllocations << "\n";
	outputTU << "The size of objects allocated are between: " << rangeObjectSize.first << " and " << rangeObjectSize.second << "\n";
	outputTU << "Allocation   format: Allocate   objectId objectSize \n";
	outputTU << "Deallocation format: Deallocate objectId \n\n";

	// Select the distribution
	std::random_device rd;
	std::uniform_int_distribution<int> distribution(rangeObjectSize.first, rangeObjectSize.second);
	// Need to add more distributions

	std::set<int> storeObjectId;
	int countAllocations = 0;

	// Allocate X objects
	for (int objectId = 0; objectId < numberObjectsAllocated && countAllocations < numberAllocations; objectId++)
	{
		outputTU << "Allocate " << objectId << " " << distribution(rd) << "\n";
		countAllocations++;
	}

	// wWile we haven't made a total of Z allocations
	while (countAllocations < numberAllocations)
	{
		// Deallocate Y random objects
		deallocateObjects(countAllocations, storeObjectId);

		// Allocate Y random objects
		for (const auto& objectId : storeObjectId)
		{
			outputTU << "Allocate " << objectId << " " << distribution(rd) << "\n";
			countAllocations++;
		}

		storeObjectId.clear();
	}

	// Deallocate all objects
	for (int it = 0; it < numberObjectsAllocated; it++)
	{
		outputTU << "Deallocate " << it << "\n";
	}

	outputTU.close();
}


void GenerateTestUnits::loadTU()
{
	inputTU.open("generatedTU.txt", std::ifstream::in);
	std::string aux, instruction;
	int objectId = 0, objectSize = 0;
	char** test = nullptr;

	for (int it = 0; it < 8; it++)
	{
		std::getline(inputTU, aux);
	}

	test = new char* [numberObjectsAllocated];
	for (int it = 0; it < numberObjectsAllocated; it++)
	{
		test[it] = nullptr;
	}

	while (inputTU >> instruction)
	{
		if (instruction == "Allocate")
		{
			inputTU >> objectId >> objectSize;
			test[objectId] = new char[objectSize];
		}
		else
		{
			inputTU >> objectId;
			delete[] test[objectId];
		}
	}

	delete[] test;
	inputTU.close();
}


/*
	Method used to deallocate Y random objects (Y is decided by us)
*/
void GenerateTestUnits::deallocateObjects(const int countAllocations, std::set<int>& storeObjectId)
{
	std::random_device rd;
	std::uniform_int_distribution<int> getNumberDealloc(rangeNumberDeallocations.first, rangeNumberDeallocations.second);
	std::uniform_int_distribution<int> getObjectId(0, numberObjectsAllocated - 1);
	// generate the number of deallocations (Y)
	int numberDeallocations = std::min(getNumberDealloc(rd), numberAllocations - countAllocations);
	int objectId = 0;

	// Deallocate Y objects
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