#include "stdafx.h"


/*
	Function used to update an element
*/
void PoolElement::updateElement(char* _address, size_t _size)
{
	this->address = _address;
	this->size = _size;
}


/*
	Overload operator< to compare the elements by the address
*/
bool operator<(const PoolElement& firstPoolEl, const PoolElement& secondPoolEl)
{
	return firstPoolEl.address < secondPoolEl.address;
}