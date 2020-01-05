#pragma once


class DiagnoseInternalFragmentation
{
public:
	DiagnoseInternalFragmentation() : totalMemoryAllocated(0), totalMemoryRequested(0), maxMemoryAllocated(0), maxMemoryRequested(0) {}

	~DiagnoseInternalFragmentation();

private:
	std::ofstream diagFile;
	int totalMemoryAllocated, totalMemoryRequested;
	int maxMemoryAllocated, maxMemoryRequested;
};