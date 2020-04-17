#include "stdafx.h"


/*
	Method using the default allocator to allocate memory in case a custom allocator wasn't choosed
*/
void* __cdecl Strategy::allocMemory(size_t aSize, int aBlockUse, char const* aFileName, int aLineNumber)
{
	// default CRT implementation
	return _malloc_dbg(aSize, aBlockUse, aFileName, aLineNumber);
}


/*
	Method using the default deallocator to deallocate memory in case a custom deallocator wasn't choosed
*/
void __cdecl Strategy::freeMemory(void* aBlock, int aBlockUse)
{
	// default CRT implementation
	_free_dbg(aBlock, aBlockUse);
}


void Strategy::serializationStrategy(std::ofstream& output)
{
	output.close();
}


void Strategy::deserializationStrategy(std::ifstream& input)
{
	input.close();
}