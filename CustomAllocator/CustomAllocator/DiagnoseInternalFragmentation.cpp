#include "stdafx.h"


/*
	Method used to calculate the fragmentation state of the custom allocator
*/
void DiagnoseInternalFragmentation::evaluateFragmentation(const std::pair<int, int>& currentState)
{
	int currMemoryAllocated = currentState.first;
	int currMemoryRequested = currentState.second;
	double fragmentationState = (double)currMemoryAllocated - currMemoryRequested;

	showFragmentationState(fragmentationState);
}