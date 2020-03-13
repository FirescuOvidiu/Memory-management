#pragma once


/*
	This class is used to evaluate the fragmentation state of the custom allocators
	It calculates the external fragmentation based on the formula:
	Fragmentation = 1 - (biggest free continuous memory)/(total free memory)
*/
class DiagnoseExternalFragmentation : public DiagnosticTools
{
public:
	void evaluateFragmentation(const std::pair<int, int> currentState) override;
};