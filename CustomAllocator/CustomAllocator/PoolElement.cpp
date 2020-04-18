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


void PoolElement::serializationPoolElement(std::ofstream& output, char* startAddress) const
{
	size_t currAddress =  address - startAddress;

	output.write(reinterpret_cast<const char*>(&currAddress), sizeof(currAddress));
	output.write(reinterpret_cast<const char*>(&size), sizeof(size));
}


void PoolElement::deserializationPoolElement(std::ifstream& input, char* startAddress)
{
	size_t currAddress = 0;

	input.read(reinterpret_cast<char*>(&currAddress), sizeof(currAddress));
	address = startAddress + currAddress;
	input.read(reinterpret_cast<char*>(&size), sizeof(size));
}