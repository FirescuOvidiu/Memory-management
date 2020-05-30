#pragma once


/*
	This class is used to evaluate the fragmentation state of the custom allocators
	It calculates the external fragmentation based on the formula:
	Fragmentation = 1 - (biggest free continuous memory)/(total free memory)
*/
class DiagnoseExternalFragmentation : public DiagnosticTools
{
public:
	DiagnoseExternalFragmentation(const int totalMemory = 0, const diagnosticType diagType = diagnosticType::No_Diagnostic) : DiagnosticTools(totalMemory, diagType) {}
	void evaluateFragmentation(const std::pair<int, int>& currentState) override;
};