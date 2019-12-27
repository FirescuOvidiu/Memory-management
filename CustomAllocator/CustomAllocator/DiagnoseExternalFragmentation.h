#pragma once


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
	int nrFragmentation;
	double avgFragmentation;
};