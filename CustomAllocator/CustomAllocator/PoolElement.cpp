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
	int lengthAddress = strlen(address);

	output.write(reinterpret_cast<const char*>(&lengthAddress), sizeof(lengthAddress));
	output.write(reinterpret_cast<const char*>(address), lengthAddress);
	output.write(reinterpret_cast<const char*>(&size), sizeof(size));
}


void PoolElement::deserializationPoolElement(std::ifstream& input)
{
	int lengthAddress = 0;

	input.read(reinterpret_cast<char*>(&lengthAddress), sizeof(lengthAddress));
	input.read(reinterpret_cast<char*>(address), lengthAddress);
	input.read(reinterpret_cast<char*>(&size), sizeof(size));
}