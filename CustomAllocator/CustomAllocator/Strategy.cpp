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