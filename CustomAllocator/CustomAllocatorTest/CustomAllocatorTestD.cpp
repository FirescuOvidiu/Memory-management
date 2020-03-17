#include "stdafx.h"
#include "CustomAllocatorTestD.h"


void testD()
{
	std::random_device rd;
	std::uniform_int_distribution<int> dist(32, 64);

	char** test;
	int size = 16;

	test = new char* [size];
	for (int i = 0; i < size; i++)
	{
		test[i] = new char[dist(rd)];
	}

	std::uniform_int_distribution<int> dist2(1, size);
	std::uniform_int_distribution<int> distPos(0, size - 1);
	int nr = 0;
	int pos = 0;
	int iterations = 1000;

	for (int it = 0; it < iterations; it++)
	{
		nr = dist2(rd);

		for (int i = 0; i < nr; i++)
		{
			pos = distPos(rd);
			delete test[pos];
			test[pos] = new char[dist(rd)];
		}
	}

	for (int i = 0; i < size; i++)
	{
		delete[] test[i];
	}
	delete[] test;
}