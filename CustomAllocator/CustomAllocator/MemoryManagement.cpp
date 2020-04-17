#include "stdafx.h"


/*
	Constructor used to choose a custom allocator based on a context
*/
MemoryManagement::MemoryManagement(const int context, const int poolSize, const diagnosticTypes diagType) : context(context), poolSize(poolSize), diagType(diagType), diagTools(nullptr)
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


void MemoryManagement::serialization(std::ofstream& output)
{
	output.write(reinterpret_cast<const char*>(&context), sizeof(context));
	output.write(reinterpret_cast<const char*>(&poolSize), sizeof(poolSize));
	output.write(reinterpret_cast<const char*>(static_cast<int>(diagType)), sizeof(int));

	(*customAllocator).serialization(output);
}


void MemoryManagement::deserialization(std::ifstream& input)
{
	(*customAllocator).deserialization(input);
}


/*
	Destructor used to destroy the custom allocator after the application ends
*/
MemoryManagement::~MemoryManagement()
{
	delete customAllocator;
}
