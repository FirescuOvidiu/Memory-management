#include "stdafx.h"
#include "CustomAllocator.h"

//----------------------------------------------------------------------------

MemoryManagement* memoryManagement = new MemoryManagement(2, 4096, diagnosticTypes::Console_Diagnostic);

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

	return (*memoryManagement).allocMemory(aSize, aBlockUse, aFileName, aLineNumber);
}

void __cdecl CustomAllocator_Free(void * aBlock, int aBlockUse, char const * /*aFileName*/, int /*aLineNumber*/)
{
	// default CRT implementation
	// _free_dbg(aBlock, aBlockUse);

	(*memoryManagement).freeMemory(aBlock, aBlockUse);
}

void evaluateFragmentationState()
{
	(*memoryManagement).evaluateFragmentation();
}

void serialization()
{
	std::ofstream output("serialization.bin", std::ofstream::out | std::ofstream::binary);

	output.close();
}

void deserialization()
{
	std::ifstream input("serialization.bin", std::ifstream::in | std::ifstream::binary);
	int context = 0, poolSize = 0, diagTypes = 0;

	delete memoryManagement;
	input.read(reinterpret_cast<char*>(&context), sizeof(context));
	input.read(reinterpret_cast<char*>(&poolSize), sizeof(poolSize));
	input.read(reinterpret_cast<char*>(&diagTypes), sizeof(diagTypes));

	memoryManagement = new MemoryManagement(context, poolSize, static_cast<diagnosticTypes>(diagTypes));
	(*memoryManagement).deserialization(input);

	input.close();
}