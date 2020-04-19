#include "stdafx.h"

char* PoolElement::startAddress = nullptr;

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


void PoolElement::setStartAddress(char* startAddresss)
{
	startAddress = startAddresss;
}


std::ostream& operator<<(std::ostream& output, const PoolElement& poolElement)
{
	size_t currAddress = poolElement.address - PoolElement::startAddress;

	output.write(reinterpret_cast<const char*>(&currAddress), sizeof(currAddress));
	output.write(reinterpret_cast<const char*>(&poolElement.size), sizeof(poolElement.size));

	return output;
}


std::istream& operator>>(std::istream& input, PoolElement& poolElement)
{
	size_t currAddress = 0;

	input.read(reinterpret_cast<char*>(&currAddress), sizeof(currAddress));
	poolElement.address = PoolElement::startAddress + currAddress;
	input.read(reinterpret_cast<char*>(&poolElement.size), sizeof(poolElement.size));

	return input;
}