#include "stdafx.h"
#include "CustomAllocator.h"

//----------------------------------------------------------------------------

MemoryManagement memoryManagement(strategyType::BestFit, 4096, diagnosticType::Console_Diagnostic, benchMarkingType::No_BenchMark);


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
	// default CRT implementation
	// return _malloc_dbg(aSize, aBlockUse, aFileName, aLineNumber);

	return memoryManagement.allocMemory(aSize, aBlockUse, aFileName, aLineNumber);
}


void __cdecl CustomAllocator_Free(void * aBlock, int aBlockUse, char const * /*aFileName*/, int /*aLineNumber*/)
{
	// default CRT implementation
	// _free_dbg(aBlock, aBlockUse);

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
}