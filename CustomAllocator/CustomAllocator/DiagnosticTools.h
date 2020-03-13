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
	DiagnosticTools() : totalMemory(0), typeFragmentation(true), diagType(diagnosticTypes::No_Diagnostic) {}

	// Method used to initialize diagnostic tools
	void initDiagnosticTools(size_t poolSize, bool typeFragmentation, diagnosticTypes diagType);

	virtual void evaluateFragmentation(const std::pair<int,int> currentState) = 0;
	void showFragmentationState(double fragmentationState);

protected:
	std::ofstream diagFile;
	bool typeFragmentation;
	int totalMemory;
	diagnosticTypes diagType;
};