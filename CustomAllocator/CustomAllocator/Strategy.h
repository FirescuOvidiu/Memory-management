#pragma once
#include "stdafx.h"


/*
	Represents the base class for all the custom allocators
*/
class Strategy
{
public:
	virtual void* __cdecl allocMemory(size_t aSize, int aBlockUse, char const* aFileName, int aLineNumber);
	virtual void __cdecl freeMemory(void* aBlock, int aBlockUse);

	virtual void serialization(std::ofstream& output);
	virtual void deserialization(std::ifstream& input);

	virtual ~Strategy() {}

protected:
	Logger log;
};