#include "stdafx.h"


MemoryManagement::MemoryManagement(const strategyType allocatorType, const int poolSize, const diagnosticType diagType, const LogType logType) : allocatorType(allocatorType), poolSize(poolSize), diagType(diagType), logType(logType)
{
	setAllocatorAndDiagnostic(allocatorType);
}


/*
	Method used to serialize an object by overloading operator<<
*/
std::ofstream& operator<<(std::ofstream& output, const MemoryManagement& memoryManager)
{
	// Serialize data members by writing their content in the output file
	Writer::writeVariable(output, static_cast<int>(memoryManager.diagType));
	Writer::writeVariable(output, memoryManager.poolSize);
	Writer::writeVariable(output, static_cast<int>(memoryManager.allocatorType));

	// Serialize the remaining objects present in the class
	output << (*memoryManager.allocator);

	return output;
}


/*
	Method used to deserialize an object by overloading operator>>
*/
std::ifstream& operator>>(std::ifstream& input, MemoryManagement& memoryManager)
{
	// Release the unique_ptr's 
	memoryManager.allocator.release();
	memoryManager.diagTools.release();

	// Deserialize data members by reading their content from the input file
	memoryManager.diagType = static_cast<diagnosticType>(Reader::readVariable<int>(input));
	memoryManager.poolSize = Reader::readVariable<decltype(memoryManager.poolSize)>(input);
	memoryManager.allocatorType = static_cast<strategyType>(Reader::readVariable<int>(input));
	memoryManager.logType = LogType::No_Log;

	// Initialaze the allocator and diagnostic tools based on the data members read
	memoryManager.setAllocatorAndDiagnostic(memoryManager.allocatorType);

	// Deserialize the remaining objects present in the class
	input >> (*memoryManager.allocator);

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

	default:
		break;
	}
}


/*
	Method used to set a strategy for the allocator and diagnostics tools
*/
void MemoryManagement::setAllocatorAndDiagnostic(const strategyType _allocatorType)
{
	switch (_allocatorType)
	{
	case strategyType::WorstFit:
		allocator = std::make_unique<WorstFit>(poolSize, logType);
		diagTools = std::make_unique<DiagnoseExternalFragmentation>((int)poolSize, diagType);
		break;

	case strategyType::BestFit:
		allocator = std::make_unique<BestFit>(poolSize, logType);
		diagTools = std::make_unique<DiagnoseExternalFragmentation>((int)poolSize, diagType);
		break;

	case strategyType::BuddySystem:
		allocator = std::make_unique<BuddySystem>(poolSize, logType);
		diagTools = std::make_unique<DiagnoseInternalFragmentation>((int)poolSize, diagType);
		break;

	case strategyType::StandardAllocator:
		allocator = std::make_unique<StandardAllocator>();
		break;
	}
}