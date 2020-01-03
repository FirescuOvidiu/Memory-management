#include "stdafx.h"


/*
	Constructor used to choose a custom allocator based on a context
*/
MemoryManagement::MemoryManagement(const int context, const int poolSize)
{
	switch (context)
	{
	case 1:
		customAllocator = new WorstFit(poolSize);
		break;

	case 2:
		customAllocator = new BuddySystem(poolSize);
		break;

	default:
		customAllocator = new Strategy;
	}
}


/*
	Destructor used to destroy the custom allocator after the application ends
*/
MemoryManagement::~MemoryManagement()
{
	delete customAllocator;
}
