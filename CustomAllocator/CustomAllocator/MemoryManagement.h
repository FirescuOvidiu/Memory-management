#pragma once
class MemoryManagement
{
public:
	MemoryManagement(const int context, const int poolSize);
public:
	Strategy* customAllocator;

};

