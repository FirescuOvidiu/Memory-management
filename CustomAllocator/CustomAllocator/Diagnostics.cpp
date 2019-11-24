#include "stdafx.h"


/*
	Updating the data members
*/
void Diagnostics::updateDiagnostics(int memoryUtilization, int maxContinuousMemory)
{
	nrMemoryUtilization++;
	nrMaxContinuousMemory++;
	avgMemoryUtilization += memoryUtilization;
	avgMaxContinuousMemory += maxContinuousMemory;
	if (maxMemoryUtilization < memoryUtilization)
	{
		maxMemoryUtilization = memoryUtilization;
	}
}


/*
	Sets the total amount of memory allocated of the memory pool
*/
void Diagnostics::setTotalMemory(int _totalMemory)
{
	this->totalMemory = _totalMemory;
}


/*
	Returns the total size of the memory pool allocated at the start of the program
*/
int Diagnostics::getTotalMemory()
{
	return totalMemory;
}


/*
	Destructor used to write the informations gathered during the program into a file
*/
Diagnostics::~Diagnostics()
{
	diagFile.open("diagnosticsFile.diag", std::ofstream::out);

	diagFile << "\n\n\t" << "<----------------------- START OF APPLICATION ----------------------->" << "\n\n";
	diagFile << "\tTotal memory allocated by the memory pool: " << totalMemory << "\n\n";
	diagFile << "\tThe maximum memory utilization during the application was: " << maxMemoryUtilization << " bytes out of " << totalMemory << " bytes (" << (float) maxMemoryUtilization * 100 / totalMemory << "%).\n";
	diagFile << "\tThe average memory utilization (allocated) during the application was: " << avgMemoryUtilization / nrMemoryUtilization << " bytes.\n\n";
	diagFile << "\tThe average of the maximum continuous memory during the application was: " << avgMaxContinuousMemory / nrMaxContinuousMemory << " bytes.\n\n";
	diagFile << "\t" << "<----------------------- END OF APPLICATION ----------------------->";

	diagFile.close();
}