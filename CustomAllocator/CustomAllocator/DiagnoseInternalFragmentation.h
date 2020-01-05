#pragma once


/*
	This class is used to evaluate the fragmentation state of the custom allocators
	It calculates the internal fragmentation based on the following method:
	The maximum amount of memory used by the allocator relative to the maximum amount of live memory
*/
class DiagnoseInternalFragmentation
{
public:
	DiagnoseInternalFragmentation() : totalMemory(0), totalMemoryAllocated(0), totalMemoryRequested(0), maxMemoryAllocated(0), maxMemoryRequested(0) {}

	~DiagnoseInternalFragmentation();

private:
	std::ofstream diagFile;
	int totalMemory;
	int totalMemoryAllocated, totalMemoryRequested;
	int maxMemoryAllocated, maxMemoryRequested;
};