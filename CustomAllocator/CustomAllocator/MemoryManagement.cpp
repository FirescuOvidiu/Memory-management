#include "stdafx.h"


/*
	Constructor used to choose a custom allocator based on a context
*/
MemoryManagement::MemoryManagement(const strategyType context, const int poolSize, const diagnosticType diagType, const benchMarkingType benchmarkType) :
	context(context), poolSize(poolSize), diagType(diagType), benchmarkType(benchmarkType), diagTools(nullptr)
{
	switch (context)
	{
	case strategyType::WorstFit:
		customAllocator = new WorstFit(poolSize);
		diagTools = new DiagnoseExternalFragmentation((int)poolSize, diagType);
		break;

	case strategyType::BestFit:
		customAllocator = new BestFit(poolSize);
		diagTools = new DiagnoseExternalFragmentation((int)poolSize, diagType);
		break;

	case strategyType::BuddySystem:
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
	int strategyType = static_cast<int>(memoryManagement.context);

	// Serialize the data members by writing their content in the output file
	output.write(reinterpret_cast<const char*>(&strategyType), sizeof(strategyType));
	output.write(reinterpret_cast<const char*>(&memoryManagement.poolSize), sizeof(memoryManagement.poolSize));
	output.write(reinterpret_cast<const char*>(&diagType), sizeof(diagType));

	// Serialize the objects present in this class
	output << (*memoryManagement.customAllocator);

	return output;
}


/*
	Method used to deserialize an object by overloading operator ">>"
*/
std::istream& operator>>(std::istream& input, MemoryManagement& memoryManagement)
{
	int newStrategyType = 0, newDiagType = 0;

	// Deserialize data members by reading their content from the input file
	input.read(reinterpret_cast<char*>(&newStrategyType), sizeof(newStrategyType));
	input.read(reinterpret_cast<char*>(&memoryManagement.poolSize), sizeof(memoryManagement.poolSize));
	input.read(reinterpret_cast<char*>(&newDiagType), sizeof(newDiagType));

	memoryManagement.diagType = static_cast<diagnosticType>(newDiagType);
	memoryManagement.context = static_cast<strategyType>(newStrategyType);
	// Delete the current allocated objects. After deleting, allocate and initialize them based on the data from the input file
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
	case strategyType::WorstFit:
		memoryManagement.customAllocator = new WorstFit(memoryManagement.poolSize);
		memoryManagement.diagTools = new DiagnoseExternalFragmentation((int)memoryManagement.poolSize, memoryManagement.diagType);
		break;

	case strategyType::BestFit:
		memoryManagement.customAllocator = new BestFit(memoryManagement.poolSize);
		memoryManagement.diagTools = new DiagnoseExternalFragmentation((int)memoryManagement.poolSize, memoryManagement.diagType);
		break;

	case strategyType::BuddySystem:
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
	if ((benchmarkType == BenchMarking_Types::No_BenchMark) || (benchmarkType == BenchMarking_Types::Deallocation_BenchMark))
	{
		return customAllocator->allocMemory(aSize, aBlockUse, aFileName, aLineNumber);
	}
	else
	{
		timer.startTimer();
		void* address = customAllocator->allocMemory(aSize, aBlockUse, aFileName, aLineNumber);
		timer.stopTimer();

		return address;
	}
}


/*
	Calls method freeMemory from Strategy class
*/
void __cdecl MemoryManagement::freeMemory(void* aBlock, int aBlockUse)
{
	if ((benchmarkType == BenchMarking_Types::No_BenchMark) || (benchmarkType == BenchMarking_Types::Allocation_BenchMark))
	{
		customAllocator->freeMemory(aBlock, aBlockUse);
	}
	else
	{
		timer.startTimer();
		customAllocator->freeMemory(aBlock, aBlockUse);
		timer.stopTimer();
	}
}


/*
	Used to evaluate the fragmentation state of the custom allocator
*/
void MemoryManagement::evaluateFragmentation()
{
	switch (context)
	{
	case strategyType::WorstFit:
		diagTools->evaluateFragmentation(static_cast<WorstFit*>(customAllocator)->getCurrentState());
		static_cast<WorstFit*>(customAllocator)->showCurrentState();
		break;

	case strategyType::BestFit:
		diagTools->evaluateFragmentation(static_cast<BestFit*>(customAllocator)->getCurrentState());
		static_cast<BestFit*>(customAllocator)->showCurrentState();
		break;

	case strategyType::BuddySystem:
		diagTools->evaluateFragmentation(static_cast<BuddySystem*>(customAllocator)->getCurrentState());
		static_cast<BuddySystem*>(customAllocator)->showCurrentState();
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