#pragma once
#include "stdafx.h"


typedef enum class Strategy_Types
{
	StandardAllocator =0,
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
	Strategy(const size_t poolSize = 0) : log(poolSize) {}

	friend std::ofstream& operator<<(std::ofstream& output, const Strategy& allocator);
	friend std::ifstream& operator>>(std::ifstream& input, Strategy& allocator);

	virtual std::ofstream& write(std::ofstream& output) const = 0;
	virtual std::ifstream& read(std::ifstream& input) = 0;

	virtual void* __cdecl allocMemory(size_t aSize, int aBlockUse, char const* aFileName, int aLineNumber) = 0;
	virtual void __cdecl freeMemory(void* aBlock, int aBlockUse) = 0;

	virtual ~Strategy() {}

protected:
	Logger log;
};