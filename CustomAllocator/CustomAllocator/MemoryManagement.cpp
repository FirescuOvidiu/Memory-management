#include "stdafx.h"


MemoryManagement::MemoryManagement(const strategyType allocatorType, const int poolSize, const diagnosticType diagType) : allocatorType(allocatorType), poolSize(poolSize), diagType(diagType)
{
	setAllocatorAndDiagnostic(allocatorType);
}


/*
	Method used to serialize an object by overloading operator<<
*/
std::ostream& operator<<(std::ostream& output, const MemoryManagement& memoryManagement)
{
	int diagType = static_cast<int>(memoryManagement.diagType);
	int strategyType = static_cast<int>(memoryManagement.allocatorType);

	// Serialize the data members by writing their content in the output file
	output.write(reinterpret_cast<const char*>(&strategyType), sizeof(strategyType));
	output.write(reinterpret_cast<const char*>(&memoryManagement.poolSize), sizeof(memoryManagement.poolSize));
	output.write(reinterpret_cast<const char*>(&diagType), sizeof(diagType));

	// Serialize the objects present in this class
	output << (*memoryManagement.allocator);

	return output;
}


/*
	Method used to deserialize an object by overloading operator>>
*/
std::istream& operator>>(std::istream& input, MemoryManagement& memoryManagement)
{
	int newStrategyType = 0, newDiagType = 0;

	// Deserialize data members by reading their content from the input file
	input.read(reinterpret_cast<char*>(&newStrategyType), sizeof(newStrategyType));
	input.read(reinterpret_cast<char*>(&memoryManagement.poolSize), sizeof(memoryManagement.poolSize));
	input.read(reinterpret_cast<char*>(&newDiagType), sizeof(newDiagType));

	memoryManagement.diagType = static_cast<diagnosticType>(newDiagType);
	memoryManagement.allocatorType = static_cast<strategyType>(newStrategyType);

	// Delete the current allocated objects. After deleting, allocate and initialize them based on the data from the input file
	memoryManagement.allocator.release();
	memoryManagement.diagTools.release();

	memoryManagement.setAllocatorAndDiagnostic(memoryManagement.allocatorType);

	// Deserialize the objects present in this class
	input >> (*memoryManagement.allocator);

	return input;
}


void* __cdecl MemoryManagement::allocMemory(size_t aSize, int aBlockUse, char const* aFileName, int aLineNumber)
{
	return allocator->allocMemory(aSize, aBlockUse, aFileName, aLineNumber);
}


void __cdecl MemoryManagement::freeMemory(void* aBlock, int aBlockUse)
{
	allocator->freeMemory(aBlock, aBlockUse);
}


/*
	Method used to evaluate and save the current state of the memory 
*/
void MemoryManagement::evaluateFragmentation()
{
	switch (allocatorType)
	{
	case strategyType::WorstFit:
		diagTools->evaluateFragmentation(static_cast<WorstFit*>(allocator.get())->getCurrentState());
		static_cast<WorstFit*>(allocator.get())->showCurrentState();
		break;

	case strategyType::BestFit:
		diagTools->evaluateFragmentation(static_cast<BestFit*>(allocator.get())->getCurrentState());
		static_cast<BestFit*>(allocator.get())->showCurrentState();
		break;

	case strategyType::BuddySystem:
		diagTools->evaluateFragmentation(static_cast<BuddySystem*>(allocator.get())->getCurrentState());
		static_cast<BuddySystem*>(allocator.get())->showCurrentState();
		break;
	}
}


/*
	Method used to set a strategy for the custom allocator and diagnostics tools
*/
void MemoryManagement::setAllocatorAndDiagnostic(const strategyType _allocatorType)
{
	switch (_allocatorType)
	{
	case strategyType::WorstFit:
		allocator = std::make_unique<WorstFit>(poolSize);
		diagTools = std::make_unique<DiagnoseExternalFragmentation>((int)poolSize, diagType);
		break;

	case strategyType::BestFit:
		allocator = std::make_unique<BestFit>(poolSize);
		diagTools = std::make_unique<DiagnoseExternalFragmentation>((int)poolSize, diagType);
		break;

	case strategyType::BuddySystem:
		allocator = std::make_unique<BuddySystem>(poolSize);
		diagTools = std::make_unique<DiagnoseInternalFragmentation>((int)poolSize, diagType);
		break;
	}
}