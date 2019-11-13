#include "stdafx.h"
#include "CustomAllocatorTestC.h"

void testC()
{
	// Case C1 X C3 C4
	int* c1 = new int(10);
	int* c2 = new int(20);
	int* c3 = new int(10);
	int* c4 = new int(20);
	delete c1;
	delete c3;
	delete c4;
	delete c2;

	// Case X C2
	c1 = new int(10);
	c2 = new int(20);
	c3 = new int(10);
	delete c2;
	delete c1;
	delete c3;

	// Case C1 X
	c1 = new int(10);
	c2 = new int(20);
	c3 = new int(30);
	delete c1;
	delete c2;
	delete c3;
}