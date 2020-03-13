#pragma once


/*
	Contains all the diagnostic tools used to evaluate the custom allocators
*/
class DiagnosticTools
{
public:
	DiagnosticTools() : totalMemory(0), typeFragmentation(true) {}

	// Method used to initialize diagnostic tools
	void initDiagnosticTools(size_t poolSize, bool typeFragmentation);
	virtual void evaluateFragmentation(const std::pair<int,int> currentState) = 0;

	void updateDiagnosticTools(const int size, const int memoryToAllocate);

	// Methods used to update diagExternal
	void updateExternalFrag(const int biggestContMemory);

	// Methods used to update diagInternal
	void updateInternalFrag(const int memoryAllocated, const int memoryRequested);

private:
	std::ofstream diagFile;
	bool typeFragmentation;
	int totalMemory;
	//DiagnoseExternalFragmentation diagExternal;  // External diagnostic of the custom allocator
	//DiagnoseInternalFragmentation diagInternal;  // Internal diagnostic of the custom allocator
};