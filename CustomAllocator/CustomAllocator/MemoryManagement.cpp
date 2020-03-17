#include "stdafx.h"


/*
	Constructor used to choose a custom allocator based on a context
*/
MemoryManagement::MemoryManagement(const int context, const int poolSize) : context(context), diagTools(nullptr)
{
	switch (context)
	{
	case 1:
		customAllocator = new WorstFit(poolSize);
		diagTools = new DiagnoseExternalFragmentation((int)poolSize, diagnosticTypes::Console_Diagnostic);
		break;

	case 2:
		customAllocator = new BuddySystem(poolSize);
		diagTools = new DiagnoseInternalFragmentation((int)poolSize, diagnosticTypes::Console_Diagnostic);
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
