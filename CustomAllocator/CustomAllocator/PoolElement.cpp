#include "stdafx.h"


void PoolElement::updateElement(char* adress, size_t size)
{
	this->adress = adress;
	this->size = size;
}

bool operator<(const PoolElement& a, const PoolElement& b)
{
	return a.adress < b.adress;
}