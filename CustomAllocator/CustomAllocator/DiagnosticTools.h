#pragma once


/*
	Contains all the diagnostic tools used to evaluate the custom allocators
*/
class DiagnosticTools
{
public:
	void updateInfoLog(size_t poolSize, char* startAddresss);
	void updateWarningLog(size_t poolSize);
	void updateErrorLog(void* block, size_t memoryToAllocate, size_t biggestContMemory, const std::string& situation);
	void increaseAllocOrDealloc(const int size);

	void initExternalFrag(const int totalMemory);
	void updateExternalFrag(const int biggestContMemory);

	void initInternalFrag(const int totalMemory);
	void updateInternalFrag(const int memoryAllocated, const int memoryRequested);

	void initDiagnostics(const int totalMemory);
	void updateDiagnostics(const int memoryToAllocate);

private:
	Logger log;			// Logger of the custom allocator
	DiagnoseExternalFragmentation diagExternal;  // External diagnostic of the custom allocator
	DiagnoseInternalFragmentation diagInternal;  // Internal diagnostic of the custom allocator
	Diagnostics diag;	// Diagnostics of the custom allocator
};