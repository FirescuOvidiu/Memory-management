#include "stdafx.h"


void DiagnosticTools::initDiagnosticTools(size_t poolSize, char* startAddress, bool typeFragmentation)
{
	// Initialize data members of the diagnostics
	diag.initDiagnostics((int)poolSize);

	if (typeFragmentation)
	{
		// Initialize data members of the external diagnostics
		diagExternal.initExternalFrag((int)poolSize);
	}
	else
	{
		// Initialize data members of the external diagnostics
		diagInternal.initInternalFrag((int)poolSize);
	}
}


void DiagnosticTools::updateDiagnosticTools(const int size, const int memoryToAllocate)
{
	// log.increaseAllocOrDealloc(size);
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
	diagInternal.updateInternalFrag(memoryAllocated, memoryRequested);
}