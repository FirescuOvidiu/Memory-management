#include "stdafx.h"


/*
	Constructor used to open the file, and initializate the data members
*/
Diagnostics::Diagnostics() : totalMemory(0), maxMemoryUtilization(0), nrMemoryUtilization(0), nrMaxContiniousMemory(0), avgMemoryUtilization(0), avgMaxContinousMemory(0)
{
	diagFile.open("DiagnosticsFile.diag", std::ofstream::out);
}


void Diagnostics::updateDiagOnAlloc(const int memoryUtilization, const int maxContinuousMemory)
{
	nrMemoryUtilization++;
	nrMaxContiniousMemory++;
	avgMemoryUtilization += memoryUtilization;
	avgMaxContinousMemory += maxContinuousMemory;
	if (maxMemoryUtilization < memoryUtilization)
	{
		maxMemoryUtilization = memoryUtilization;
	}
}


void Diagnostics::updateDiagOnDealloc(const int memoryUtilisation, const int maxContinuousMemory)
{
}


Diagnostics::~Diagnostics()
{
	diagFile.close();
}