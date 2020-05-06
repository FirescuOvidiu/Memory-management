#include "stdafx.h"


/*
	Constructor used to choose a custom allocator based on a context
*/
MemoryManagement::MemoryManagement(const int context, const int poolSize, const diagnosticTypes diagType, const benchMarkingTypes benchmarkType) :
	context(context), poolSize(poolSize), diagType(diagType), benchmarkType(benchmarkType), diagTools(nullptr)
{
	switch (context)
	{
	case 1:
		customAllocator = new WorstFit(poolSize);
		diagTools = new DiagnoseExternalFragmentation((int)poolSize, diagType);
		break;

	case 2:
		customAllocator = new BuddySystem(poolSize);
		diagTools = new DiagnoseInternalFragmentation((int)poolSize, diagType);
		break;

	default:
		customAllocator = new Strategy;
	}
}


/*
	Method used to serialize an object by overloading the operator "<<"
*/
std::ostream& operator<<(std::ostream& output, const MemoryManagement& memoryManagement)
{
	int diagType = static_cast<int>(memoryManagement.diagType);

	// Serialize the data members by writing their content in the output file
	output.write(reinterpret_cast<const char*>(&memoryManagement.context), sizeof(memoryManagement.context));
	output.write(reinterpret_cast<const char*>(&memoryManagement.poolSize), sizeof(memoryManagement.poolSize));
	output.write(reinterpret_cast<const char*>(&diagType), sizeof(int));

	// Serialize the objects present in this class
	output << (*memoryManagement.customAllocator);

	return output;
}


/*
	Method used to deserialize an object by overloading operator ">>"
*/
std::istream& operator>>(std::istream& input, MemoryManagement& memoryManagement)
{
	int diagType = 0;

	// Deserialize data members by reading their content from the input file
	input.read(reinterpret_cast<char*>(&memoryManagement.context), sizeof(memoryManagement.context));
	input.read(reinterpret_cast<char*>(&memoryManagement.poolSize), sizeof(memoryManagement.poolSize));
	input.read(reinterpret_cast<char*>(&diagType), sizeof(int));

	memoryManagement.diagType = static_cast<diagnosticTypes>(diagType);

	// Delete the current allocated objects and allocate and initialize them 
	// based on the data from the input file
	if (memoryManagement.customAllocator != nullptr)
	{
		delete memoryManagement.customAllocator;
	}
	if (memoryManagement.diagTools != nullptr)
	{
		delete memoryManagement.diagTools;
	}

	switch (memoryManagement.context)
	{
	case 1:
		memoryManagement.customAllocator = new WorstFit(memoryManagement.poolSize);
		memoryManagement.diagTools = new DiagnoseExternalFragmentation((int)memoryManagement.poolSize, memoryManagement.diagType);
		break;

	case 2:
		memoryManagement.customAllocator = new BuddySystem(memoryManagement.poolSize);
		memoryManagement.diagTools = new DiagnoseInternalFragmentation((int)memoryManagement.poolSize, memoryManagement.diagType);
		break;

	default:
		memoryManagement.customAllocator = new Strategy;
	}

	// Deserialize the objects present in this class
	input >> (*memoryManagement.customAllocator);

	return input;
}


/*
	Calls method allocMemory from Strategy class
*/
void* __cdecl MemoryManagement::allocMemory(size_t aSize, int aBlockUse, char const* aFileName, int aLineNumber)
{
	if (benchmarkType != BenchMarking_Types::No_BenchMark)
	{
		return customAllocator->allocMemory(aSize, aBlockUse, aFileName, aLineNumber);
	}
	else
	{
		if (benchmarkType != BenchMarking_Types::Deallocation_BenchMark)
		{
			timer.startTimer();
			void* address = customAllocator->allocMemory(aSize, aBlockUse, aFileName, aLineNumber);
			timer.stopTimer();

			return address;
		}
	}
}


/*
	Calls method freeMemory from Strategy class
*/
void __cdecl MemoryManagement::freeMemory(void* aBlock, int aBlockUse)
{
	if (benchmarkType != BenchMarking_Types::No_BenchMark)
	{
		customAllocator->freeMemory(aBlock, aBlockUse);
	}
	else
	{
		if (benchmarkType != BenchMarking_Types::Allocation_BenchMark)
		{
			timer.startTimer();
			customAllocator->freeMemory(aBlock, aBlockUse);
			timer.stopTimer();
		}
	}
}


/*
	Used to evaluate the fragmentation state of the custom allocator
*/
void MemoryManagement::evaluateFragmentation()
{
	switch (context)
	{
	case 1:
		diagTools->evaluateFragmentation(static_cast<WorstFit*>(customAllocator)->getCurrentState());
		break;

	case 2:
		diagTools->evaluateFragmentation(static_cast<BuddySystem*>(customAllocator)->getCurrentState());
		break;
	}
}


/*
	Destructor used to destroy the custom allocator after the application ends
*/
MemoryManagement::~MemoryManagement()
{
	if (customAllocator != nullptr)
	{
		delete customAllocator;
	}
	if (diagTools != nullptr)
	{
		delete diagTools;
	}
}