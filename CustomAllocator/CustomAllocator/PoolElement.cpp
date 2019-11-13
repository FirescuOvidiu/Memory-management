#include "stdafx.h"

/*
	Function used to update an element
*/
void PoolElement::updateElement(char* _adress, size_t _size)
{
	this->adress = _adress;
	this->size = _size;
}

/*
	Overload operator< to compare the elements by the adress
*/
bool operator<(const PoolElement& a, const PoolElement& b)
{
	return a.adress < b.adress;
}