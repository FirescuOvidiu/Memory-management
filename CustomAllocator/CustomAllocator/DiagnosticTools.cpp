#include "stdafx.h"


void DiagnosticTools::initDiagnosticTools(size_t poolSize, bool typeFragmentation)
{
	this->totalMemory = (int)poolSize;
	this->typeFragmentation = typeFragmentation;
}


void DiagnosticTools::updateDiagnosticTools(const int size, const int memoryToAllocate)
{
	// diag.updateDiagnostics(diag.getTotalMemory() - log.totalMemoryAvailable, memoryToAllocate);
}


//----------------------------------------------------------------------------
void DiagnosticTools::updateExternalFrag(const int biggestContMemory)
{
	//diagExternal.updateExternalFrag(log.totalMemoryAvailable, biggestContMemory);
}


//----------------------------------------------------------------------------
void DiagnosticTools::updateInternalFrag(const int memoryAllocated, const int memoryRequested)
{
	//diagInternal.updateInternalFrag(memoryAllocated, memoryRequested);
}