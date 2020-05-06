#include "stdafx.h"


/*
		Method used to serialize an object by overloading the operator<<
		Calls `write` method because we need a virtual function to serialize
*/
std::ostream& operator<<(std::ostream& output, const Strategy& customAllocator)
{
	return customAllocator.write(output);
}


/*
	Method used to deserialize an object by overloading operator>>
	Calls `read` method because we need a virtual function to deserialize
*/
std::istream& operator>>(std::istream& input, Strategy& customAllocator)
{
	return customAllocator.read(input);
}


/*
	Virtual function used to serialize an object will be implemented in the derived classes
*/
std::ostream& Strategy::write(std::ostream& output) const
{
	return output;
}


/*
	Virtual function used to deserialize an object will be implemented in the derived classes
*/
std::istream& Strategy::read(std::istream& input)
{
	return input;
}


/*
	Method using the default allocator to allocate memory in case a custom allocator wasn't choosed
*/
void* __cdecl Strategy::allocMemory(size_t aSize, int /*aBlockUse*/, char const* /*aFileName*/, int /*aLineNumber*/)
{
	// default CRT implementation
	return _malloc_dbg(aSize, aBlockUse, aFileName, aLineNumber);
}


/*
	Method using the default deallocator to deallocate memory in case a custom deallocator wasn't choosed
*/
void __cdecl Strategy::freeMemory(void* aBlock, int /*aBlockUse*/)
{
	// default CRT implementation
	_free_dbg(aBlock, aBlockUse);
}