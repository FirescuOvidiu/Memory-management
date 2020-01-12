#pragma once
#include "stdafx.h"


/*
	This class is used to evaluate the state of the memory,
	provides informations about the memory utilization, blocks allocated
*/
class Diagnostics
{
public:
	Diagnostics() : totalMemory(0), maxMemoryUtil(0), nrMemoryUtil(0), avgMemoryUtil(0), countBlocksAllocated(100) {}

	void initDiagnostics(const int _totalMemory);
	void updateDiagnostics(const int memoryUtil, const int memoryToAllocate);

	int getTotalMemory() const;

	~Diagnostics();

private:
	// Evaluates the state of the memory, contains informations about memory 
	std::ofstream diagFile;
	int totalMemory;
	std::vector<int> countBlocksAllocated;

	int nrMemoryUtil;			// Used to calculate average memory utilization
	double avgMemoryUtil;		// Average memory utilization
	int maxMemoryUtil;			// Maximum memory utilization
};