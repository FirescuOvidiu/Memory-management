#include "stdafx.h"


/*
	Method used to update the data members used for storing informations about memory
*/
void Diagnostics::updateMemoryInf(const int memoryUtilization, const int maxContinuousMemory)
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
	Method used to initialize members totalMemory and segmentLength
*/
void Diagnostics::initializeDiagnostics(const int _totalMemory)
{
	this->totalMemory = _totalMemory;
	this->segmentLength = _totalMemory / 20;
}


/*
	Method used to update the data members used for storing informations about segments
*/
void Diagnostics::updateSegmentInf(const std::list<PoolElement>& mAvailable)
{
	int currNumberSegments = 0;
	for (auto it = mAvailable.begin(); it != mAvailable.end(); it++)
	{
		if ((*it).size <= segmentLength)
		{
			currNumberSegments++;
		}
	}

	numberSegments += currNumberSegments;
	if (currNumberSegments > maxNumberSegments)
	{
		maxNumberSegments = currNumberSegments;
	}
	countAvgSegments += currNumberSegments;
}


/*
	Returns the total size of the memory pool allocated at the start of the program
*/
int Diagnostics::getTotalMemory() const
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