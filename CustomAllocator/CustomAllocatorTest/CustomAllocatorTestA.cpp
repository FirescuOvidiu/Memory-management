#include "CustomAllocatorTestA.h"
#include "stdafx.h"

void testA()
{
	A* pA = new A();
	pA->b++;
	std::cout << pA->b << std::endl;
	delete pA;

	A* pA2 = new A(true);
	pA2->b++;
	std::cout << pA2->b << std::endl;
	delete pA2;

	A* parrA = new A[5];
	parrA[2].c = true;
	delete[] parrA;

	const int NO_STR = 5;
	if (auto p = (std::string*)malloc(NO_STR * sizeof(std::string)))
	{
		std::cout << sizeof(std::string);
		for (int i = 0; i != NO_STR; ++i) // populate the array
		{
			void* placement = p + i;
			new(placement) std::string(4, (char)('a' + i));
			std::cout << "\n"<<sizeof(std::string(4, (char)('a' + i)));
		}

		for (int j = 0; j != NO_STR; ++j)
			std::cout << "p[" << j << "] == " << p[j] << '\n';

		for (int i = NO_STR; i != 0; --i) // clean up
			p[i - 1].std::string::~string();

		free(p);
	}
}