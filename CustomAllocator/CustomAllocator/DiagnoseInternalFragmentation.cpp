#include "stdafx.h"


/*
	Method used to initialize data member totalMemory
*/
void DiagnoseInternalFragmentation::initInternalFrag(const int _totalMemory)
{
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

}