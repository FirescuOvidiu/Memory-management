#pragma once

class Strategy
{
public:
	virtual void* __cdecl allocMemory(size_t aSize, int aBlockUse, char const* /*aFileName*/, int /*aLineNumber*/) = 0;
	virtual void __cdecl freeMemory(void* aBlock, int /*aBlockUse*/) = 0;
};