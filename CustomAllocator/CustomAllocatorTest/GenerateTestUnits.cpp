#include "stdafx.h"


const int GenerateTestUnits::numberObjectsAllocated = 100;
const char GenerateTestUnits::allocationNotation = 'A', GenerateTestUnits::deallocationNotation = 'D';


/*
	Method used to generate test units based on a distribution
	How we generate the test units:
		- allocate X objects at the start (X is decided by us)
		- while we haven't made a total of Z allocations: (Z is decided by us)
			- deallocate Y random objects (Y is random)
			- allocate   Y random objects
		- deallocate all objects

	The allocation and deallocation format used in file is:
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

		outputTU.write(reinterpret_cast<const char*>(&allocationNotation), sizeof(allocationNotation));
		outputTU.write(reinterpret_cast<const char*>(&objectId), sizeof(objectId));
		outputTU.write(reinterpret_cast<const char*>(&generatedNumber), sizeof(generatedNumber));

		countAllocations++;
	}

	// wWile we haven't made a total of Z allocations
	while (countAllocations < numberAllocations)
	{
		// Deallocate Y random objects
		deallocateRandomObjects(countAllocations, storeObjectId);

		// Allocate Y random objects
		for (const auto& objectId : storeObjectId)
		{
			generatedNumber = (int)std::round(distribution(rd)) + rangeObjectSize.first;

			outputTU.write(reinterpret_cast<const char*>(&allocationNotation), sizeof(allocationNotation));
			outputTU.write(reinterpret_cast<const char*>(&objectId), sizeof(objectId));
			outputTU.write(reinterpret_cast<const char*>(&generatedNumber), sizeof(generatedNumber));

			countAllocations++;
		}

		storeObjectId.clear();
	}

	// Deallocate all objects
	for (int it = 0; it < numberObjectsAllocated; it++)
	{
		outputTU.write(reinterpret_cast<const char*>(&deallocationNotation), sizeof(deallocationNotation));
		outputTU.write(reinterpret_cast<const char*>(&it), sizeof(it));
	}

	outputTU.close();
}


/*
	Method used to load the test unit generated
*/
void GenerateTestUnits::loadTU()
{
	inputTU.open("generatedBinaryTU.bin", std::ifstream::in | std::ifstream::binary);

	int objectId = 0, objectSize = 0, offset = 0;
	char instruction = {};
	char* objects[numberObjectsAllocated];

	for (int it = 0; it < numberObjectsAllocated; it++)
	{
		objects[it] = nullptr;
	}

	readGeneratedTest();

	while (offset < fileLength)
	{
		instruction = *reinterpret_cast<char*>(buffer + offset);
		offset += 1;
		if (instruction == allocationNotation)
		{
			// Allocate new object
			objectId = *reinterpret_cast<int*>(buffer + offset);
			offset += 4;
			objectSize = *reinterpret_cast<int*>(buffer + offset);
			objects[objectId] = new char[objectSize];
		}
		else
		{
			// Deallocate object
			objectId = *reinterpret_cast<int*>(buffer + offset);
			delete[] objects[objectId];
		}
		offset += 4;
		if (offset == fileLength - 5 * numberObjectsAllocated)
		{
			evaluateFragmentationState();
		}
	}
}


/*
	Method used to convert a generated test unit binary file into a text file
*/
void GenerateTestUnits::convertBinaryFile()
{
	inputTU.open("generatedBinaryTU.bin", std::ifstream::in | std::ifstream::binary);
	outputTU.open("generatedTU.txt", std::ofstream::out | std::ofstream::binary);
	int objectId = 0, objectSize = 0, countInstructions = 0, instructionNumber = numberAllocations * 2;
	char instruction = {};

	while (countInstructions < instructionNumber)
	{
		inputTU.read(reinterpret_cast<char*>(&instruction), sizeof(instruction));
		outputTU << instruction << " ";

		if (instruction == allocationNotation)
		{
			inputTU.read(reinterpret_cast<char*>(&objectId), sizeof(objectId));
			inputTU.read(reinterpret_cast<char*>(&objectSize), sizeof(objectSize));
			outputTU << objectId << " " << objectSize << "\n";
		}
		else
		{
			inputTU.read(reinterpret_cast<char*>(&objectId), sizeof(objectId));
			outputTU << objectId << "\n";
		}

		countInstructions++;
	}

	inputTU.close();
	outputTU.close();
}


/*
	Method used to deallocate Y random objects (Y is decided by us)
*/
void GenerateTestUnits::deallocateRandomObjects(const int countAllocations, std::set<int>& storeObjectId)
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

		outputTU.write(reinterpret_cast<const char*>(&deallocationNotation), sizeof(deallocationNotation));
		outputTU.write(reinterpret_cast<const char*>(&objectId), sizeof(objectId));
		storeObjectId.insert(objectId);
	}
}


/*
	Method used to read from file the generated test unit and save it into the buffer
*/
void GenerateTestUnits::readGeneratedTest()
{
	inputTU.seekg(0, inputTU.beg);
	inputTU.read(buffer, fileLength);
	inputTU.close();
}