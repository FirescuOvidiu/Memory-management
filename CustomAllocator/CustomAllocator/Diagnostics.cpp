#include "stdafx.h"


/*
	Method used to initialize members totalMemory and segmentLength
*/
void Diagnostics::initializeDiagnostics(const int poolSize)
{
	this->nrMaxContinuousMemory++;
	this->avgMaxContinuousMemory += poolSize;
	this->totalMemory = poolSize;
	this->segmentLength = poolSize / 16;
}


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
	Method used to update the data members used for storing informations about segments
*/
void Diagnostics::updateSegmentInf(const std::list<PoolElement>& mAvailable)
{
	int currNumberSegments = 0;
	int memoryLost = 0;

	for (auto it = mAvailable.begin(); it != mAvailable.end(); it++)
	{
		if ((*it).size <= segmentLength)
		{
			currNumberSegments++;
			memoryLost += (int) (*it).size;
		}
	}

	nrSegments++;
	numberSegments += currNumberSegments;
	if (currNumberSegments > maxNumberSegments)
	{
		maxNumberSegments = currNumberSegments;
		maxMemoryLost = memoryLost;
	}
	avgSegments += currNumberSegments;
	avgMemoryLost += memoryLost;
}


/*
	Returns the total size of the memory pool allocated at the start of the program
*/
int Diagnostics::getTotalMemory() const
{
	return totalMemory;
}


/*
	Destructor used to write the informations gathered during the program into the file diagnosticsFile.diag
*/
Diagnostics::~Diagnostics()
{
	diagFile.open("diagnosticsFile.diag", std::ofstream::out);

	double calculatProc = totalMemory / 100;

	avgMemoryUtilization /= nrMemoryUtilization;
	avgMaxContinuousMemory /= nrMaxContinuousMemory;
	avgSegments /= nrSegments;
	avgMemoryLost /= nrSegments;

	diagFile << std::setprecision(2) << std::fixed << "\n\n\t" << "<----------------------- START OF APPLICATION ----------------------->" << "\n\n";
	diagFile << "\tTotal memory allocated by the memory pool: " << totalMemory << " bytes.\n\n";
	diagFile << "\tThe maximum memory utilization during the application was: " << maxMemoryUtilization << " bytes out of " << totalMemory << " bytes (" << (double)maxMemoryUtilization / calculatProc << "%).\n";
	diagFile << "\tThe average memory utilization (allocated) during the application was: " << avgMemoryUtilization << " bytes (" << avgMemoryUtilization / calculatProc << "%).\n";
	diagFile << "\tThe average of the maximum continuous memory during the application was: " << avgMaxContinuousMemory << " bytes (" << avgMaxContinuousMemory / calculatProc << "%).\n\n";
	diagFile << "\tA segment represent a continuous block of memory which is too small to be used in allocations.\n";
	diagFile << "\tThe length of the segment for this application was: " << segmentLength << " bytes (6.25%).\n";
	diagFile << "\tThe number of segments during the application was: " << numberSegments << " segments.\n";
	diagFile << "\tThe average number of segments during the application was: " << avgSegments << " segments (" << avgSegments * segmentLength << " bytes).\n";
	diagFile << "\tThe average number of bytes lost during the application was: " << avgMemoryLost << " bytes.\n";
	diagFile << "\tThe maximum number of segments during the application was: " << maxNumberSegments << " segments ("<<maxNumberSegments*segmentLength<<" bytes).\n";
	diagFile << "\tThe maximum number of segments during the application was: " << maxMemoryLost << " bytes.\n\n";
	diagFile << "\t" << "<----------------------- END OF APPLICATION ----------------------->";

	diagFile.close();
}