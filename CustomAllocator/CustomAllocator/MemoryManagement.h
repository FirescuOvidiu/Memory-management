#pragma once


/*
	Used for choosing a custom allocator based on a context
*/
class MemoryManagement
{
public:
	MemoryManagement(const int context, const int poolSize);

	void* __cdecl allocMemory(size_t aSize, int aBlockUse, char const* aFileName, int aLineNumber);
	void __cdecl freeMemory(void* aBlock, int aBlockUse);

	void evaluateFragmentation();

	~MemoryManagement();

private:
	DiagnosticTools* diagTools;
	Strategy* customAllocator;
	int context;
};