#include "stdafx.h"


/*
	Method using the default allocator to allocate memory in case a custom allocator wasn't choosed
*/
void* __cdecl Strategy::allocMemory(size_t aSize, int aBlockUse, char const* aFileName, int aLineNumber)
{
	return _malloc_dbg(aSize, aBlockUse, aFileName, aLineNumber);
}


/*
	Method using the default deallocator to deallocate memory in case a custom deallocator wasn't choosed
*/
void __cdecl Strategy::freeMemory(void* aBlock, int aBlockUse)
{
	_free_dbg(aBlock, aBlockUse);
}
