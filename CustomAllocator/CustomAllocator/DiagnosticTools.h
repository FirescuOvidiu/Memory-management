#pragma once


typedef enum class Diagnostic_Types
{
	No_Diagnostic = 1,
	Console_Diagnostic = 2,
	File_Diagnostic = 3
}diagnosticType;


/*
	Contains all the diagnostic tools used to evaluate the custom allocators
*/
class DiagnosticTools
{
public:
	DiagnosticTools(const int totalMemory = 0, const diagnosticType diagType = diagnosticType::No_Diagnostic) : totalMemory(totalMemory), diagType(diagType) {}

	// Method used to evaluate the fragmentation state of the custom allocators
	virtual void evaluateFragmentation(const std::pair<int,int>& currentState) = 0;

	virtual ~DiagnosticTools() {}

protected:
	// Method used to display the fragmentation state of the custom allocators
	void showFragmentationState(double fragmentationState);

protected:
	int totalMemory;

private:
	std::ofstream diagFile;
	diagnosticType diagType;
};