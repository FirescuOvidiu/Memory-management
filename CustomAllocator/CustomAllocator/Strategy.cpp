#include "stdafx.h"

void* __cdecl Strategy::allocMemory(size_t aSize, int aBlockUse, char const* aFileName, int aLineNumber)
{
	return _malloc_dbg(aSize, aBlockUse, aFileName, aLineNumber);
}

void __cdecl Strategy::freeMemory(void* aBlock, int aBlockUse)
{
	_free_dbg(aBlock, aBlockUse);
}
