#include "stdafx.h"


//----------------------------------------------------------------------------
void DiagnosticTools::updateInfoLog(size_t poolSize, char* startAddresss)
{
	log.updateInfoLog(poolSize, startAddresss);
}


void DiagnosticTools::updateWarningLog(size_t poolSize)
{
	log.updateWarningLog(poolSize);
}


void DiagnosticTools::updateErrorLog(void* block, size_t memoryToAllocate, size_t biggestContMemory, const std::string& situation)
{
	log.updateErrorLog(block, memoryToAllocate, biggestContMemory, situation);

	log.~Logger();
	diag.~Diagnostics();
	diagExternal.~DiagnoseExternalFragmentation();
	diagInternal.~DiagnoseInternalFragmentation();
}


void DiagnosticTools::increaseAllocOrDealloc(const int size)
{
	log.increaseAllocOrDealloc(size);
}


//----------------------------------------------------------------------------
void DiagnosticTools::initExternalFrag(const int totalMemory)
{
	diagExternal.initExternalFrag(totalMemory);
}


void DiagnosticTools::updateExternalFrag(const int biggestContMemory)
{
	diagExternal.updateExternalFrag(log.totalMemoryAvailable, biggestContMemory);
}


//----------------------------------------------------------------------------
void DiagnosticTools::initInternalFrag(const int totalMemory)
{
	diagInternal.initInternalFrag(totalMemory);
}


void DiagnosticTools::updateInternalFrag(const int memoryAllocated, const int memoryRequested)
{
	diagInternal.updateInternalFrag(memoryAllocated, memoryRequested);
}


//----------------------------------------------------------------------------
void DiagnosticTools::initDiagnostics(const int totalMemory)
{
	diag.initDiagnostics(totalMemory);
}


void DiagnosticTools::updateDiagnostics(const int memoryToAllocate)
{
	diag.updateDiagnostics(diag.getTotalMemory() - log.totalMemoryAvailable, memoryToAllocate);
}