#include "stdafx.h"


void DiagnoseInternalFragmentation::evaluateFragmentation(const std::pair<int, int>& currentState)
{
	int currMemoryAllocated = currentState.first;
	int currMemoryRequested = currentState.second;
	double fragmentationState = currMemoryAllocated - currMemoryRequested;

	showFragmentationState(fragmentationState);
}