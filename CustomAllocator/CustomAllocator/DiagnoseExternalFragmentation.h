#pragma once


/*
	This class is used to evaluate the fragmentation state of the custom allocators
	It calculates the external fragmentation based on the formula:
	Fragmentation = 1 - (biggest free continuous memory)/(total free memory)
*/
class DiagnoseExternalFragmentation
{
public:
	DiagnoseExternalFragmentation() : totalMemory(0), nrFragmentation(0), avgFragmentation(0) {}

	void initExternalFrag(const int _totalMemory);
	void updateExternalFrag(const int totalMemoryAvailable, const int biggestContMemory);

	~DiagnoseExternalFragmentation();

private:
	std::ofstream diagFile;
	int totalMemory;
	int nrFragmentation;		// Used to calculate the average fragmentation
	double avgFragmentation;
};