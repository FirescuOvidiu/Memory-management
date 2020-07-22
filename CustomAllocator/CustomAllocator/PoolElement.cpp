#include "stdafx.h"

char* PoolElement::startAddress = nullptr;

/*
	Overload operator "<" to compare two objects by their address
*/
bool PoolElement::operator<(const PoolElement& poolElement) const
{
	return this->address < poolElement.address;
}


/*
	Method used to serialize an object by overloading the operator "<<"
*/
std::ofstream& operator<<(std::ofstream& output, const PoolElement& poolElement)
{
	// We will serialize the current address based on the startAddress
	size_t currAddress = poolElement.address - PoolElement::startAddress;

	// Serialize the data members by writing their content in the output file
	output.write(reinterpret_cast<const char*>(&currAddress), sizeof(currAddress));
	output.write(reinterpret_cast<const char*>(&poolElement.size), sizeof(poolElement.size));

	return output;
}


/*
	Method used to deserialize an object by overloading operator ">>"
*/
std::ifstream& operator>>(std::ifstream& input, PoolElement& poolElement)
{
	size_t currAddress = 0;

	// Deserialize data members by reading their content from the input file
	input.read(reinterpret_cast<char*>(&currAddress), sizeof(currAddress));
	poolElement.address = PoolElement::startAddress + currAddress;
	input.read(reinterpret_cast<char*>(&poolElement.size), sizeof(poolElement.size));

	return input;
}


/*
	Function used to update an element
*/
void PoolElement::updateElement(char* _address, size_t _size)
{
	this->address = _address;
	this->size = _size;
}


/*
	Setter used to set a value for startAddress
*/
void PoolElement::setStartAddress(char* startAddresss)
{
	startAddress = startAddresss;
}