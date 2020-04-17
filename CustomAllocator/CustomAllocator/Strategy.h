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

	virtual void serializationStrategy(std::ofstream& output);
	virtual void deserializationStrategy(std::ifstream& input);

	virtual ~Strategy() {}

protected:
	Logger log;
};