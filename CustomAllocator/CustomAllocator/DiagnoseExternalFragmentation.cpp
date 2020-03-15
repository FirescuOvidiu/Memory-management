#include "stdafx.h"


void DiagnoseExternalFragmentation::evaluateFragmentation(const std::pair<int, int>& currentState)
{
	int biggestContMemory = currentState.first;
	int memoryAvailable = currentState.second;
	double fragmentationState = (1 - (double)biggestContMemory / memoryAvailable) * 100 * totalMemory;

	showFragmentationState(fragmentationState);
}