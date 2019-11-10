#pragma once

struct A
{
	int a;
	int b;
	bool c;
	char r[48];

	A() = default;
	A(bool v) : a(5), b(v ? 1 : 0), c(v) {}
};

void testA();