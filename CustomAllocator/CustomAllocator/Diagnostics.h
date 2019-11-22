#pragma once
#include "stdafx.h"


/*
	This class is used to evaluate the fragmentation state of the memory pool
	It also provides informations about the memory utilization
*/
class Diagnostics
{
public:
	Diagnostics() : totalMemory(0), maxMemoryUtilization(0), nrMemoryUtilization(0), nrMaxContiniousMemory(0), avgMemoryUtilization(0), avgMaxContinousMemory(0) {}

	void updateDiagnostics(const int memoryUtilization, const int maxContinuousMemory);
	void setTotalMemory(int _totalMemory);

	~Diagnostics();

private:
	std::ofstream diagFile;
	int totalMemory;
	int maxMemoryUtilization;
	int nrMemoryUtilization;			// Used to calculate the average memory utilization
	int nrMaxContiniousMemory;		   // Used to calculate the average of the maximum continuous memory
	double avgMemoryUtilization;
	double avgMaxContinousMemory;

	// need to invent a metric to evaluate the fragmentation state of the memory pool
};