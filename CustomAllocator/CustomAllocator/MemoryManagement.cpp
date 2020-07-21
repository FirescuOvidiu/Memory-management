#include "stdafx.h"


/*
	Constructor used to choose a custom allocator based on a context
*/
MemoryManagement::MemoryManagement(const strategyType context, const int poolSize, const diagnosticType diagType) : context(context), poolSize(poolSize), diagType(diagType)
{
	switch (context)
	{
	case strategyType::WorstFit:
		customAllocator = std::make_unique<WorstFit>(poolSize);
		diagTools = std::make_unique<DiagnoseExternalFragmentation>((int)poolSize, diagType);
		break;

	case strategyType::BestFit:
		customAllocator = std::make_unique<BestFit>(poolSize);
		diagTools = std::make_unique<DiagnoseExternalFragmentation>((int)poolSize, diagType);
		break;

	case strategyType::BuddySystem:
		customAllocator = std::make_unique<BuddySystem>(poolSize);
		diagTools = std::make_unique<DiagnoseInternalFragmentation>((int)poolSize, diagType);
		break;
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
	memoryManagement.customAllocator.release();
	memoryManagement.diagTools.release();

	switch (memoryManagement.context)
	{
	case strategyType::WorstFit:
		memoryManagement.customAllocator = std::make_unique<WorstFit>(memoryManagement.poolSize);
		memoryManagement.diagTools = std::make_unique<DiagnoseExternalFragmentation>((int)memoryManagement.poolSize, memoryManagement.diagType);
		break;

	case strategyType::BestFit:
		memoryManagement.customAllocator = std::make_unique<BestFit>(memoryManagement.poolSize);
		memoryManagement.diagTools = std::make_unique<DiagnoseExternalFragmentation>((int)memoryManagement.poolSize, memoryManagement.diagType);
		break;

	case strategyType::BuddySystem:
		memoryManagement.customAllocator = std::make_unique<BuddySystem>(memoryManagement.poolSize);
		memoryManagement.diagTools = std::make_unique<DiagnoseInternalFragmentation>((int)memoryManagement.poolSize, memoryManagement.diagType);
		break;
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
	return customAllocator->allocMemory(aSize, aBlockUse, aFileName, aLineNumber);
}


/*
	Calls method freeMemory from Strategy class
*/
void __cdecl MemoryManagement::freeMemory(void* aBlock, int aBlockUse)
{
	customAllocator->freeMemory(aBlock, aBlockUse);
}


/*
	Used to evaluate the fragmentation state of the custom allocator
*/
void MemoryManagement::evaluateFragmentation()
{
	switch (context)
	{
	case strategyType::WorstFit:
		diagTools->evaluateFragmentation(static_cast<WorstFit*>(customAllocator.get())->getCurrentState());
		static_cast<WorstFit*>(customAllocator.get())->showCurrentState();
		break;

	case strategyType::BestFit:
		diagTools->evaluateFragmentation(static_cast<BestFit*>(customAllocator.get())->getCurrentState());
		static_cast<BestFit*>(customAllocator.get())->showCurrentState();
		break;

	case strategyType::BuddySystem:
		diagTools->evaluateFragmentation(static_cast<BuddySystem*>(customAllocator.get())->getCurrentState());
		static_cast<BuddySystem*>(customAllocator.get())->showCurrentState();
		break;
	}
}