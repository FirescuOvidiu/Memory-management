#include "stdafx.h"


void DiagnoseExternalFragmentation::initExternalFrag(const int _totalMemory)
{
	totalMemory = _totalMemory;
}


void DiagnoseExternalFragmentation::updateExternalFrag(const int totalMemoryAvailable, const int biggestContMemory)
{
	double currFragmentation{ 0 };

	currFragmentation = 1 - (double)biggestContMemory / totalMemoryAvailable;
	avgFragmentation += currFragmentation;
	nrFragmentation++;
}