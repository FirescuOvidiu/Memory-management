#include "CustomAllocatorTestB.h"
#include "stdafx.h"


void testB()
{
	//int* tt = new int[2000];
	//*tt = 1;
	// 3. void* __cdecl operator new(size_t aSize)
	int* alfa = new int(10);
	// 13. void __cdecl operator delete(void* aBlock, size_t /*aSize*/) noexcept
	delete alfa;

	B* b1 = new B(10, 'C');
	b1->a = 5;
	std::cout << b1->a << "\n\n";
	delete b1;

	// 5. void * __cdecl operator new[](size_t aSize)
	B* b2 = new B[10];
	b2[5].a = b2[5].a - 10;
	std::cout << b2[5].a << "\n";
	std::cout << b2[4].a << "\n\n";
	// 14. void __cdecl operator delete[](void * aBlock, size_t /*aSize*/) noexcept
	delete[]b2;

	std::vector<B*> b3;
	b3.push_back(new B(1, 'A'));
	b3.push_back(new B[10]);
	std::cout << *b3[0]->b << "\n";
	std::cout << (b3[1] + 9)->a << "\n\n";

	int** b4;
	b4 = new int* [10];
	for (int i = 0; i < 10; i++)
	{
		b4[i] = new int[5];
	}
	for (int i = 0; i < 10; i++)
	{
		// 11 .void __cdecl operator delete[](void * aBlock) noexcept
		delete[] b4[i];
	}
	delete[] b4;

	int* b5 = (int*)malloc(sizeof(int) * 22);
	*(b5 + 1) = 99;
	std::cout << *(b5 + 1) << "\n\n";
	free(b5);

	// 6. void * __cdecl operator new[](size_t aSize, std::nothrow_t const &) noexcept
	int* b6 = new (std::nothrow) int[5];
	// 4. void * __cdecl operator new(size_t aSize, std::nothrow_t const &) noexcept
	int* b7 = new (std::nothrow) int(11);
	delete[] b6;
	delete b7;

	int* b8 = new int(10);
	delete b8;
	delete b8;
}