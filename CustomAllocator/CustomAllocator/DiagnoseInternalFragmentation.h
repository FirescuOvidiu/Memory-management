#pragma once


/*
	This class is used to evaluate the fragmentation state of the custom allocators
	It calculates the internal fragmentation based on the following method:
	The amount of memory used by the allocator relative to the amount of live memory
*/
class DiagnoseInternalFragmentation final : public DiagnosticTools
{
public:
	DiagnoseInternalFragmentation(const int totalMemory = 0, const diagnosticType diagType = diagnosticType::No_Diagnostic) : DiagnosticTools(totalMemory, diagType) {}
	void evaluateFragmentation(const std::pair<int, int>& currentState) override;
};