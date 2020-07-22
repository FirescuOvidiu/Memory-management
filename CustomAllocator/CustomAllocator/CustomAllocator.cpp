#include "stdafx.h"
#include "CustomAllocator.h"

//----------------------------------------------------------------------------

MemoryManagement memoryManagement(strategyType::StandardAllocator, 134217728, diagnosticType::Console_Diagnostic);


void * __cdecl CustomAllocator_New(size_t aSize, int aBlockUse, char const * aFileName, int aLineNumber)
{
	return CustomAllocator_Malloc(aSize, aBlockUse, aFileName, aLineNumber);
}


void __cdecl CustomAllocator_Delete(void * aBlock, int aBlockUse, char const * aFileName, int aLineNumber) noexcept
{
	CustomAllocator_Free(aBlock, aBlockUse, aFileName, aLineNumber);
}


void * __cdecl CustomAllocator_Malloc(size_t aSize, int aBlockUse, char const * aFileName, int aLineNumber)
{
	return memoryManagement.allocMemory(aSize, aBlockUse, aFileName, aLineNumber);
}


void __cdecl CustomAllocator_Free(void * aBlock, int aBlockUse, char const * /*aFileName*/, int /*aLineNumber*/)
{
	memoryManagement.freeMemory(aBlock, aBlockUse);
}


void evaluateFragmentationState()
{
	memoryManagement.evaluateFragmentation();
}


void serialization()
{
	std::ofstream output("serialization.bin", std::ofstream::out | std::ofstream::binary);

	output << memoryManagement;

	output.close();
}


void deserialization()
{
	std::ifstream input("serialization.bin", std::ifstream::in | std::ifstream::binary);

	input >> memoryManagement;

	input.close();
	evaluateFragmentationState();
	exit(0);
}