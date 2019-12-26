#pragma once

class DiagnoseExternalFragmentation
{
public:
	DiagnoseExternalFragmentation() : totalMemory(0), nrFragmentation(0), avgFragmentation(0) {}

private:
	std::ofstream diagFile;
	int totalMemory;
	int nrFragmentation;
	double avgFragmentation;
};