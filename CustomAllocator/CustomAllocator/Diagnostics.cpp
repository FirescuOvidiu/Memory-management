#include "stdafx.h"


/*
	Method used to initialize members totalMemory and segmentLength
*/
void Diagnostics::initializeDiagnostics(const int poolSize)
{
	this->nrMaxContMemory++;
	this->avgMaxContMemory += poolSize;
	this->totalMemory = poolSize;
	this->segmentLength = poolSize / 16;
}


/*
	Method used to update data members used for storing informations about memory
*/
void Diagnostics::updateMemoryInf(const int memoryUtil, const int maxContMemory)
{
	nrMemoryUtil++;
	nrMaxContMemory++;
	avgMemoryUtil += memoryUtil;
	avgMaxContMemory += maxContMemory;
	if (maxMemoryUtil < memoryUtil)
	{
		maxMemoryUtil = memoryUtil;
	}
}


/*
	Method used to update data members used for storing informations about segments
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
		maxMemoryWasted = memoryLost;
	}
	avgSegments += currNumberSegments;
	avgMemoryWasted += memoryLost;
}


/*
	Returns total size of the memory pool allocated at the start of the program
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

	double calculatProcents = totalMemory / 100;

	avgMemoryUtil /= nrMemoryUtil;
	avgMaxContMemory /= nrMaxContMemory;
	avgSegments /= nrSegments;
	avgMemoryWasted /= nrSegments;

	diagFile << std::setprecision(2) << std::fixed << "\n\n\t" << "<----------------------- START OF APPLICATION ----------------------->" << "\n\n";
	diagFile << "\tTotal memory allocated by the memory pool: " << totalMemory << " bytes.\n\n";
	diagFile << "\tThe maximum memory utilization during the application was: " << maxMemoryUtil << " bytes out of " << totalMemory << " bytes (" << (double)maxMemoryUtil / calculatProcents << "%).\n";
	diagFile << "\tThe average memory utilization (allocated) during the application was: " << avgMemoryUtil << " bytes (" << avgMemoryUtil / calculatProcents << "%).\n";
	diagFile << "\tThe average of the maximum continuous memory during the application was: " << avgMaxContMemory << " bytes (" << avgMaxContMemory / calculatProcents << "%).\n\n";
	diagFile << "\tA segment represent a continuous block of memory which is too small to be used in allocations.\n";
	diagFile << "\tThe length of the segment for this application was: " << segmentLength << " bytes (6.25%).\n";
	diagFile << "\tThe number of segments during the application was: " << numberSegments << " segments.\n";
	diagFile << "\tThe average number of segments during the application was: " << avgSegments << " segments (" << avgSegments * segmentLength << " bytes).\n";
	diagFile << "\tThe average number of bytes wasted during the application was: " << avgMemoryWasted << " bytes.\n";
	diagFile << "\tThe maximum number of segments during the application was: " << maxNumberSegments << " segments ("<<maxNumberSegments*segmentLength<<" bytes).\n";
	diagFile << "\tThe maximum number of bytes wasted during the application was: " << maxMemoryWasted << " bytes.\n\n";
	diagFile << "\t" << "<----------------------- END OF APPLICATION ----------------------->";

	diagFile.close();
}