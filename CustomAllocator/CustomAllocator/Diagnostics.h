#pragma once
#include "stdafx.h"


// Will modify this class later on


/*
	This class is used to evaluate the fragmentation state of the custom allocators
	It also provides informations about the memory utilization
*/
class Diagnostics
{
public:
	Diagnostics() : totalMemory(0), maxMemoryUtil(0), nrMemoryUtil(0), avgMemoryUtil(0), nrMaxContMemory(0), avgMaxContMemory(0), countBlocksAllocated(100) {}

	void initializeDiagnostics(const int _totalMemory);
	void updateMemoryInf(const int memoryUtil, const int maxContMemory, const int memoryToAllocate);

	int getTotalMemory() const;

	~Diagnostics();

private:
	std::vector<int> countBlocksAllocated;
	// Evaluates the state of the memory, contains informations about memory 
	std::ofstream diagFile;
	int totalMemory;
	int maxMemoryUtil;			// Maximum memory utilization

	int nrMemoryUtil;			// Used to calculate average memory utilization
	int nrMaxContMemory;		// Used to calculate average of the maximum continuous memory
	double avgMemoryUtil;		// Average memory utilization
	double avgMaxContMemory;    // Average maximum continuous memory
};
