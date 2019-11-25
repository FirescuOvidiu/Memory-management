#include "stdafx.h"
#include "CustomAllocatorTestD.h"


void testD()
{
	std::random_device rd;
	std::uniform_int_distribution<int> dist(1, 80);

	int** test;
	test = new int* [10];
	for (int i = 0; i < 10; i++)
	{
		test[i] = new int[dist(rd)];
	}

	std::uniform_int_distribution<int> dist2(1, 10);
	std::uniform_int_distribution<int> distPos(0, 9);
	int nr = 0;
	int pos = 0;
	int iterations = 10000;
	
	for (int it = 0; it < iterations; it++)
	{
		nr = dist2(rd);

		for (int i = 0; i < nr; i++)
		{
			pos = distPos(rd);
			delete test[pos];
			test[pos] = new int[dist(rd)];
		}
	}

	for (int i = 0; i < 10; i++)
	{
		delete[] test[i];
	}
	delete[] test;
}