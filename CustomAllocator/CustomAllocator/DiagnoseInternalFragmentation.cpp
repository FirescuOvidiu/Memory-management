#include "stdafx.h"


/*
	Method used to initialize data member totalMemory
*/
void DiagnoseInternalFragmentation::initInternalFrag(const int _totalMemory)
{
	diagFile.open("diagInternalFrag.diag", std::ofstream::out);
	totalMemory = _totalMemory;
}


/*
	Method used to update data members used to calculate the fragmentation
*/
void DiagnoseInternalFragmentation::updateInternalFrag(const int memoryAllocated, const int memoryRequested)
{
	totalMemoryAllocated += memoryAllocated;
	totalMemoryRequested += memoryRequested;

	if (totalMemoryAllocated > maxMemoryAllocated) maxMemoryAllocated = totalMemoryAllocated;
	if (totalMemoryRequested > maxMemoryRequested) maxMemoryRequested = totalMemoryRequested;
}


/*
	Destructor used to write informations gathered during the application into the file diagInternalFrag.diag
*/
DiagnoseInternalFragmentation::~DiagnoseInternalFragmentation()
{
	if (diagFile.is_open())
	{
		diagFile << std::setprecision(2) << std::fixed << "\n\n\t" << "<----------------------- START OF APPLICATION ----------------------->" << "\n\n";
		diagFile << "\tTotal memory allocated by the memory pool: " << totalMemory << " bytes.\n\n";
		diagFile << "\tThe average number of bytes wasted during the application was: " << maxMemoryAllocated - maxMemoryRequested << " bytes.\n";
		diagFile << "\tFragmentation during the application was:" << ((double)(maxMemoryAllocated - maxMemoryRequested) * 100) / totalMemory << "%.\n\n";
		diagFile << "\t" << "<----------------------- END OF APPLICATION ----------------------->";

		diagFile.close();
	}
}