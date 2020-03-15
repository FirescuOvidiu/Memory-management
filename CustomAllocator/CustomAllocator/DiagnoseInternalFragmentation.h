#pragma once


/*
	This class is used to evaluate the fragmentation state of the custom allocators
	It calculates the internal fragmentation based on the following method:
	The maximum amount of memory used by the allocator relative to the maximum amount of live memory
*/
class DiagnoseInternalFragmentation : public DiagnosticTools
{
public:
	void evaluateFragmentation(const std::pair<int, int>& currentState) override;
};