#include "stdafx.h"

const int GenerateTestUnits::numberObjectsAllocated = 10;

/*
	Method used to generate test units based on a distribution
	How we generate the test units:
		- allocate X objects at the start (X is decided by us)
		- while we haven't made a total of Z allocations: (Z is decided by us)
			- deallocate Y random objects (Y is random)
			- allocate   Y random objects
		- deallocate all objects

	The llocation and deallocation format used in file is:
	Allocation   format: A objectId objectSize
	Deallocation format: D objectId

*/
void GenerateTestUnits::generateTU()
{
	outputTU.open("generatedBinaryTU.bin", std::ofstream::out | std::ofstream::binary);

	// Select the distribution
	std::random_device rd;
	//std::vector<double> intervals{ 0, (double)rangeObjectSize.second - rangeObjectSize.first - (rangeObjectSize.second - rangeObjectSize.first) / 2, (double)rangeObjectSize.second - rangeObjectSize.first };
	//std::vector<double> weights{ 1,0,1 };

	std::uniform_int_distribution<int> distribution(0, rangeObjectSize.second - rangeObjectSize.first);		// Uniform distribution
	//std::binomial_distribution<> distribution(rangeObjectSize.second - rangeObjectSize.first, 0.5);	   // Bernoulli distribution
	//std::poisson_distribution<> distribution(rangeObjectSize.second - rangeObjectSize.first - (rangeObjectSize.second - rangeObjectSize.first) / 2);			  // Rate-based distribution
	//std::normal_distribution<> distribution(rangeObjectSize.second - rangeObjectSize.first - (rangeObjectSize.second - rangeObjectSize.first) / 2, 8);			// Normal distribution
	//std::piecewise_linear_distribution<> distribution(intervals.begin(), intervals.end(), weights.begin());		// Piecewise distribution
	// Add more distributions

	std::set<int> storeObjectId;
	int countAllocations = 0, generatedNumber;

	// Allocate X objects
	for (int objectId = 0; objectId < numberObjectsAllocated && countAllocations < numberAllocations; objectId++)
	{
		generatedNumber = (int)std::round(distribution(rd)) + rangeObjectSize.first;
		outputTU.write("A", sizeof(char));
		outputTU.write(reinterpret_cast<const char*>(&objectId), sizeof(int));
		outputTU.write(reinterpret_cast<const char*>(&generatedNumber), sizeof(int));
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
			generatedNumber = (int)std::round(distribution(rd)) + rangeObjectSize.first;
			outputTU.write("A", 1);
			outputTU.write(reinterpret_cast<const char*>(&objectId), sizeof(int));
			outputTU.write(reinterpret_cast<const char*>(&generatedNumber), sizeof(int));
			countAllocations++;
		}

		storeObjectId.clear();
	}

	// Deallocate all objects
	for (int it = 0; it < numberObjectsAllocated; it++)
	{
		outputTU.write("D", sizeof(char));
		outputTU.write(reinterpret_cast<const char*>(&it), sizeof(int));
	}

	outputTU.close();
}


/*
	Method used to load the test unit generated
*/
void GenerateTestUnits::loadTU()
{
	inputTU.open("generatedBinaryTU.bin", std::ifstream::in | std::ifstream::binary);
	int objectId = 0, objectSize = 0;
	char instruction;
	char* test[numberObjectsAllocated];

	for (int it = 0; it < numberObjectsAllocated; it++)
	{
		test[it] = nullptr;
	}

	while (inputTU.read(&instruction, sizeof(char)))
	{
		if (instruction == 'A')
		{
			// Allocate new object
			inputTU.read((char*)(&objectId), sizeof(int));
			inputTU.read((char*)(&objectSize), sizeof(int));
			test[objectId] = new char[objectSize];
		}
		else
		{
			// Deallocate object
			inputTU.read((char*)(&objectId), sizeof(int));
			delete[] test[objectId];
		}
	}

	inputTU.close();
}


/*
	Method used to convert a generated test unit binary file into a text file
*/
void GenerateTestUnits::convertBinaryFile()
{
	inputTU.open("generatedBinaryTU.bin", std::ifstream::in | std::ifstream::binary);
	outputTU.open("generatedTU.txt", std::ofstream::out | std::ofstream::binary);
	int objectId = 0, objectSize = 0;
	char instruction;

	while (inputTU.read(&instruction, sizeof(char)))
	{
		outputTU << instruction << " ";
		if (instruction == 'A')
		{
			inputTU.read((char*)(&objectId), sizeof(int));
			inputTU.read((char*)(&objectSize), sizeof(int));
			outputTU << " " << objectId << " " << objectSize << "\n";
		}
		else
		{
			inputTU.read((char*)(&objectId), sizeof(int));
			outputTU << " " << objectId << "\n";
		}
	}

	inputTU.close();
	outputTU.close();
}


/*
	Method used to deallocate Y random objects (Y is decided by us)
*/
void GenerateTestUnits::deallocateObjects(const int countAllocations, std::set<int>& storeObjectId)
{
	std::random_device rd;
	std::uniform_int_distribution<int> getNumberDealloc(rangeNumberDeallocations.first, rangeNumberDeallocations.second);
	std::uniform_int_distribution<int> getObjectId(0, numberObjectsAllocated - 1);
	// Generate the number of deallocations (Y)
	int numberDeallocations = std::min(getNumberDealloc(rd), numberAllocations - countAllocations);
	int objectId = 0;

	// Deallocate Y objects
	for (int it = 0; it < numberDeallocations; it++)
	{
		do
		{
			objectId = getObjectId(rd);
		} while ((storeObjectId.find(objectId) != storeObjectId.end()));

		outputTU.write("D", sizeof(char));
		outputTU.write(reinterpret_cast<const char*>(&objectId), sizeof(int));
		storeObjectId.insert(objectId);
	}
}