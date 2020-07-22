#include "stdafx.h"


std::ofstream& StandardAllocator::write(std::ofstream& output) const
{
	return output;
}


std::ifstream& StandardAllocator::read(std::ifstream& input)
{
	return input;
}


void* __cdecl StandardAllocator::allocMemory(size_t aSize, int aBlockUse, char const* aFileName, int aLineNumber)
{
	// default CRT implementation
	return _malloc_dbg(aSize, aBlockUse, aFileName, aLineNumber);
}


void __cdecl StandardAllocator::freeMemory(void* aBlock, int aBlockUse)
{
	// default CRT implementation
	_free_dbg(aBlock, aBlockUse);

}