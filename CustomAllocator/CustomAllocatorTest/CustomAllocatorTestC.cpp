#include "stdafx.h"
#include "CustomAllocatorTestC.h"

void testC()
{
	int* c1 = new int(10);
	int* c2 = new int(20);
	delete c1;
	delete c2;
}