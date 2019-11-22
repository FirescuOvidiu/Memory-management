#pragma once
#include "stdafx.h"


/*
	This class is used to evaluate the fragmentation state of the memory pool
	It also provides informations about the memory utilization
*/
class Diagnostics
{
public:
	Diagnostics();

	void updateDiagOnAlloc(const int memoryUtilization, const int maxContinuousMemory);
	void updateDiagOnDealloc(const int memoryUtilization, const int maxContinuousMemory);

	~Diagnostics();

public:
	std::ofstream diagFile;
	int totalMemory;
	int maxMemoryUtilization;
	int nrMemoryUtilization;			// Used to calculate the average memory utilization
	int nrMaxContiniousMemory;		   // Used to calculate the average of the maximum continuous memory
	double avgMemoryUtilization;
	double avgMaxContinousMemory;

	// need to invent a metric to evaluate the fragmentation state of the memory pool
};