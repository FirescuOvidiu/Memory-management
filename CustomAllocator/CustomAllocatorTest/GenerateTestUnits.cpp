#include "stdafx.h"


const int GenerateTestUnits::numberObjectsAllocated = 100;


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
	//std::binomial_distribution<> distribution(rangeObjectSize.second - rangeObjectSize.first, 0.6);	   // Bernoulli distribution
	//std::poisson_distribution<> distribution(rangeObjectSize.second - rangeObjectSize.first - (rangeObjectSize.second - rangeObjectSize.first) / 2);			  // Rate-based distribution
	//std::normal_distribution<> distribution(rangeObjectSize.second - rangeObjectSize.first - (rangeObjectSize.second - rangeObjectSize.first) / 2, 8);			// Normal distribution
	//std::piecewise_linear_distribution<> distribution(intervals.begin(), intervals.end(), weights.begin());		// Piecewise distribution
	// Add more distributions

	std::set<int> deallocatedObjectsId;
	AllocationObject allocationObject;
	DeallocationObject deallocationObject;
	int countAllocations = 0;


	// Allocate X objects
	for (allocationObject.id = 0; allocationObject.id < numberObjectsAllocated && countAllocations < numberAllocations; allocationObject.id++)
	{
		allocationObject.size = (int)std::round(distribution(rd)) + rangeObjectSize.first;
		allocationObject.write(outputTU);
		countAllocations++;
	}

	// wWile we haven't made a total of Z allocations
	while (countAllocations < numberAllocations)
	{
		// Deallocate Y random objects
		deallocateRandomObjects(countAllocations, deallocatedObjectsId);

		// Allocate Y random objects
		for (const auto& objectId : deallocatedObjectsId)
		{
			allocationObject.id = objectId;
			allocationObject.size = (int)std::round(distribution(rd)) + rangeObjectSize.first;
			allocationObject.write(outputTU);
			countAllocations++;
		}

		deallocatedObjectsId.clear();
	}

	// Deallocate all objects
	for (deallocationObject.id = 0; deallocationObject.id < numberObjectsAllocated; deallocationObject.id++)
	{
		deallocationObject.write(outputTU);
	}

	outputTU.close();
}


/*
	Method used to load the test unit generated
*/
void GenerateTestUnits::loadTU()
{
	char* objects[numberObjectsAllocated];
	DeallocationObject deallocationObject;
	AllocationObject allocationObject;
	char instruction = {};
	int offset = 0;

	initializeObjects(objects);
	readGeneratedTest();

	while (offset < fileLength)
	{
		instruction = *reinterpret_cast<char*>(buffer + offset);
		offset += 1;

		if (instruction == allocationObject.notation)
		{
			// Allocate object
			allocationObject.get(buffer, offset);
			objects[allocationObject.id] = new char[allocationObject.size];
		}
		else
		{
			// Deallocate object
			deallocationObject.get(buffer, offset);
			delete[] objects[deallocationObject.id];
		}

		offset += 4;
		if (offset == fileLength - 5 * numberObjectsAllocated)
		{
			evaluateFragmentationState();
			serialization();
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
	DeallocationObject deallocationObject;
	AllocationObject allocationObject;
	int countInstructions = 0, instructionNumber = numberAllocations * 2;
	char instruction = {};

	while (countInstructions < instructionNumber)
	{
		inputTU.read(reinterpret_cast<char*>(&instruction), sizeof(instruction));
		outputTU << instruction << " ";

		if (instruction == allocationObject.notation)
		{
			allocationObject.read(inputTU);
			outputTU << allocationObject.id << " " << allocationObject.size << "\n";
		}
		else
		{
			deallocationObject.read(inputTU);
			outputTU << deallocationObject.id << "\n";
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
	DeallocationObject deallocationObject;

	// Deallocate Y objects
	for (int it = 0; it < numberDeallocations; it++)
	{
		do
		{
			deallocationObject.id = getObjectId(rd);
		} while ((storeObjectId.find(deallocationObject.id) != storeObjectId.end()));

		storeObjectId.insert(deallocationObject.id);
		deallocationObject.write(outputTU);
	}
}


/*
	Method used to read from file the generated test unit and save it into the buffer
*/
void GenerateTestUnits::readGeneratedTest()
{
	inputTU.open("generatedBinaryTU.bin", std::ifstream::in | std::ifstream::binary);
	inputTU.seekg(0, inputTU.beg);
	inputTU.read(buffer, fileLength);
	inputTU.close();
}


/*
	Method used to initialize objects
*/
void GenerateTestUnits::initializeObjects(char* objects[])
{
	for (int it = 0; it < numberObjectsAllocated; it++)
	{
		objects[it] = nullptr;
	}
}