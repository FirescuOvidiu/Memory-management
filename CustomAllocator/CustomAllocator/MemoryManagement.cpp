#include "stdafx.h"

MemoryManagement::MemoryManagement(const int context, const int poolSize)
{
	switch (context)
	{
	case 1:
		customAllocator = new MemoryPool(poolSize);

	default:
		customAllocator = new Strategy;
	}
}
