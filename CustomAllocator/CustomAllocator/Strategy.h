#pragma once
#include "stdafx.h"

class Strategy
{

public:
	virtual void* __cdecl allocMemory(size_t aSize, int aBlockUse, char const* aFileName, int aLineNumber);
	virtual void __cdecl freeMemory(void* aBlock, int aBlockUse);

	virtual ~Strategy() {}
};