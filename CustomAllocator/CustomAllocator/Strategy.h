#pragma once
#include "stdafx.h"


typedef enum class Strategy_Types
{
	WorstFit = 1,
	BuddySystem = 2,
	BestFit = 3
}strategyType;


/*
	Represents the base class for all the custom allocators
*/
class Strategy
{
public:
	friend std::ostream& operator<<(std::ostream& output, const Strategy& customAllocator);
	friend std::istream& operator>>(std::istream& input, Strategy& customAllocator);

	virtual std::ostream& write(std::ostream& output) const;
	virtual std::istream& read(std::istream& input);

	virtual void* __cdecl allocMemory(size_t aSize, int aBlockUse, char const* aFileName, int aLineNumber);
	virtual void __cdecl freeMemory(void* aBlock, int aBlockUse);

	virtual ~Strategy() {}

protected:
	Logger log;
};