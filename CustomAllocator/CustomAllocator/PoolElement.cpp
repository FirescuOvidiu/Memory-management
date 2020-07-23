#include "stdafx.h"


char* PoolElement::startAddress = nullptr;


bool PoolElement::operator<(const PoolElement& block) const
{
	return this->address < block.address;
}


/*
	Method used to serialize an object by overloading the operator<<
*/
std::ofstream& operator<<(std::ofstream& output, const PoolElement& block)
{
	// Serialize the data members by writing their content in the output file
	Writer::writeVariable(output, block.address - PoolElement::startAddress);
	Writer::writeVariable(output, block.size);

	return output;
}


/*
	Method used to deserialize an object by overloading operator>>
*/
std::ifstream& operator>>(std::ifstream& input, PoolElement& block)
{
	// Deserialize data members by reading their content from the input file
	block.address = PoolElement::startAddress + Reader::readVariable<size_t>(input);
	block.size = Reader::readVariable<decltype(block.size)>(input);

	return input;
}


void PoolElement::updateElement(char* _address, const size_t _size)
{
	this->address = _address;
	this->size = _size;
}


void PoolElement::setStartAddress(char* _startAddress)
{
	startAddress = _startAddress;
}