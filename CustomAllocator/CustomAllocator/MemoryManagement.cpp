#include "stdafx.h"

MemoryManagement::MemoryManagement(const int context, const int poolSize)
{
	switch (context)
	{
	case 1:
		customAllocator = new MemoryPool(poolSize);
		break;

	default:
		customAllocator = new Strategy;
	}
}

MemoryManagement::~MemoryManagement()
{
	delete customAllocator;
}
