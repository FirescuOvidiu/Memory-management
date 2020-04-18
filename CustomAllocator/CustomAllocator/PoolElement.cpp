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
	Overload operator "<" to compare the two elements by their address
*/
bool PoolElement::operator<(const PoolElement& poolElement) const
{
	return this->address < poolElement.address;
}


void PoolElement::serializationPoolElement(std::ofstream& output) const
{
	output.close();
}


void PoolElement::deserializationPoolElement(std::ifstream& input)
{
	input.close();
}