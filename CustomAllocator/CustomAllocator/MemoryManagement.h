#pragma once
class MemoryManagement
{
public:
	MemoryManagement(const int context, const int poolSize);
	~MemoryManagement();

public:
	Strategy* customAllocator;

};

