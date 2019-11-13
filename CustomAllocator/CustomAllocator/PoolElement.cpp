#include "stdafx.h"

/*
	Function used to update an element
*/
void PoolElement::updateElement(char* adress, size_t size)
{
	this->adress = adress;
	this->size = size;
}

/*
	Overload operator< to compare the elements by the adress
*/
bool operator<(const PoolElement& a, const PoolElement& b)
{
	return a.adress < b.adress;
}