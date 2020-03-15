#pragma once


typedef enum class Diagnostic_Types
{
	No_Diagnostic = 1,
	Console_Diagnostic = 2,
	File_Diagnostic = 3
}diagnosticTypes;


/*
	Contains all the diagnostic tools used to evaluate the custom allocators
*/
class DiagnosticTools
{
public:
	DiagnosticTools() : totalMemory(0), diagType(diagnosticTypes::No_Diagnostic) {}

	// Method used to initialize diagnostic tools
	void initDiagnosticTools(const size_t poolSize, const diagnosticTypes _diagType);

	// Method used to evaluate the fragmentation state of the custom allocators
	virtual void evaluateFragmentation(const std::pair<int,int>& currentState) = 0;

protected:
	void showFragmentationState(double fragmentationState);

protected:
	int totalMemory;

private:
	std::ofstream diagFile;
	diagnosticTypes diagType;
};