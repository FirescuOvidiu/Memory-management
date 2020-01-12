#pragma once


/*
	Contains all the diagnostic tools used to evaluate the custom allocators
*/
class DiagnosticTools
{
public:
	// Method used to initialize diagnostic tools
	void initDiagnosticTools(size_t poolSize, char* startAddress);

	// Methods used to update log
	void updateWarningLog(size_t poolSize);
	void updateErrorLog(void* block, size_t memoryToAllocate, size_t biggestContMemory, const std::string& situation);
	void increaseAllocOrDealloc(const int size);

	// Methods used to update diagExternal
	void updateExternalFrag(const int biggestContMemory);

	// Methods used to update diagInternal
	void updateInternalFrag(const int memoryAllocated, const int memoryRequested);

	// Methods used to update diag
	void updateDiagnostics(const int memoryToAllocate);

private:
	Logger log;			// Logger of the custom allocator
	DiagnoseExternalFragmentation diagExternal;  // External diagnostic of the custom allocator
	DiagnoseInternalFragmentation diagInternal;  // Internal diagnostic of the custom allocator
	Diagnostics diag;	// Diagnostics of the custom allocator
};