#include "stdafx.h"


void Diagnostics::updateDiagnostics(const int memoryUtilization, const int maxContinuousMemory)
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

void Diagnostics::setTotalMemory(int _totalMemory)
{
	this->totalMemory = _totalMemory;
}


Diagnostics::~Diagnostics()
{
	diagFile.open("diagnosticsFile.diag", std::ofstream::out);

	diagFile << "\tTotal memory allocated by the memory pool: " << totalMemory << "\n\n";

	diagFile.close();
}