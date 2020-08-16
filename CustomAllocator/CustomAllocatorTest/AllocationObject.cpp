#include "stdafx.h"


const char AllocationObject::notation = 'A';


void AllocationObject::write(std::ofstream& outputTU) const
{
	outputTU.write(reinterpret_cast<const char*>(&notation), sizeof(notation));
	outputTU.write(reinterpret_cast<const char*>(&id), sizeof(id));
	outputTU.write(reinterpret_cast<const char*>(&size), sizeof(size));
}


void AllocationObject::read(std::ifstream& inputTU)
{
	inputTU.read(reinterpret_cast<char*>(&id), sizeof(id));
	inputTU.read(reinterpret_cast<char*>(&size), sizeof(size));
}


void AllocationObject::get(std::array<char, fileLength>& buffer, int& offset)
{
	id = *reinterpret_cast<int*>(buffer.data() + offset);
	offset += 4;
	size = *reinterpret_cast<int*>(buffer.data() + offset);
}