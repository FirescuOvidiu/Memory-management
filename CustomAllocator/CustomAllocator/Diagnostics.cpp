#include "stdafx.h"


/*
	Method used to initialize members totalMemory
*/
void Diagnostics::initDiagnostics(const int poolSize)
{
	this->totalMemory = poolSize;
}


/*
	Method used to update data members used for storing informations about memory
*/
void Diagnostics::updateDiagnostics(const int memoryUtil, const int memoryToAllocate)
{
	nrMemoryUtil++;
	avgMemoryUtil += memoryUtil;
	if (maxMemoryUtil < memoryUtil)
	{
		maxMemoryUtil = memoryUtil;
	}

	if (memoryToAllocate != -1)
	{
		countBlocksAllocated[(int)(memoryToAllocate * 100 / totalMemory)]++;
	}
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

	diagFile << std::setprecision(2) << std::fixed << "\n\n\t" << "<----------------------- START OF APPLICATION ----------------------->" << "\n\n";
	diagFile << "\tTotal memory allocated by the memory pool: " << totalMemory << " bytes.\n\n";
	diagFile << "\tThe maximum memory utilization during the application was: " << maxMemoryUtil << " bytes out of " << totalMemory << " bytes (" << (double)maxMemoryUtil / calculatProcents << "%).\n";
	diagFile << "\tThe average memory utilization (allocated) during the application was: " << avgMemoryUtil << " bytes (" << avgMemoryUtil / calculatProcents << "%).\n\n";
	diagFile << "\t" << "<----------------------- END OF APPLICATION ----------------------->";

	diagFile.close();
}