#include "stdafx.h"


/*
	Constructor used to choose a custom allocator based on a context
*/
MemoryManagement::MemoryManagement(const int context, const int poolSize, const diagnosticTypes diagType) : 
	context(context), poolSize(poolSize), diagType(diagType), diagTools(nullptr)
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
	delete customAllocator;
}


std::ostream& operator<<(std::ostream& output, const MemoryManagement& memoryManagement)
{
	int diagType = static_cast<int>(memoryManagement.diagType);

	output.write(reinterpret_cast<const char*>(&memoryManagement.context), sizeof(memoryManagement.context));
	output.write(reinterpret_cast<const char*>(&memoryManagement.poolSize), sizeof(memoryManagement.poolSize));
	output.write(reinterpret_cast<const char*>(&diagType), sizeof(int));

	output << (*memoryManagement.customAllocator);
	return output;
}


std::istream& operator>>(std::istream& input, MemoryManagement& memoryManagement)
{
	int diagType = 0;

	input.read(reinterpret_cast<char*>(&memoryManagement.context), sizeof(memoryManagement.context));
	input.read(reinterpret_cast<char*>(&memoryManagement.poolSize), sizeof(memoryManagement.poolSize));
	input.read(reinterpret_cast<char*>(&diagType), sizeof(int));

	memoryManagement.diagType = static_cast<diagnosticTypes>(diagType);

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

	input >> (*memoryManagement.customAllocator);
	return input;
}
