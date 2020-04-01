#include "stdafx.h"


/*
	Method used to calculate the fragmentation state of the custom allocator
*/
void DiagnoseExternalFragmentation::evaluateFragmentation(const std::pair<int, int>& currentState)
{
	int biggestContMemory = currentState.first;
	int memoryAvailable = currentState.second;
	double fragmentationState = (1 - (double)biggestContMemory / memoryAvailable) * totalMemory;

	showFragmentationState(fragmentationState);
}