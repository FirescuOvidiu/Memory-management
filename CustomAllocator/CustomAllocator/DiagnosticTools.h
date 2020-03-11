#pragma once


/*
	Contains all the diagnostic tools used to evaluate the custom allocators
*/
class DiagnosticTools
{
public:
	// Method used to initialize diagnostic tools
	void initDiagnosticTools(size_t poolSize, bool typeFragmentation);
	void updateDiagnosticTools(const int size, const int memoryToAllocate);

	void evaluateFragmentation();

	// Methods used to update diagExternal
	void updateExternalFrag(const int biggestContMemory);

	// Methods used to update diagInternal
	void updateInternalFrag(const int memoryAllocated, const int memoryRequested);

private:
	DiagnoseExternalFragmentation diagExternal;  // External diagnostic of the custom allocator
	DiagnoseInternalFragmentation diagInternal;  // Internal diagnostic of the custom allocator
};