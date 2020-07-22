#include "stdafx.h"


/*
		Method used to serialize an object by overloading the operator<<
*/
std::ofstream& operator<<(std::ofstream& output, const Strategy& allocator)
{
	return allocator.write(output);
}


/*
	Method used to deserialize an object by overloading operator>>
*/
std::ifstream& operator>>(std::ifstream& input, Strategy& allocator)
{
	return allocator.read(input);
}