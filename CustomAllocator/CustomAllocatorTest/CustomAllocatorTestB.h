#pragma once

struct B
{
	int a;
	int* b;
	char c;
	B(int a, char c) : a(a), c(c) 
	{
		b = new int(10);
	}
	B() : a(100), c('T') 
	{
		b = new int(20);
	}
	~B()
	{
		delete b;
	}
};


void testB();