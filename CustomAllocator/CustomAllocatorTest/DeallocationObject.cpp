#include "stdafx.h"


const char DeallocationObject::notation = 'D';


void DeallocationObject::write(std::ofstream& outputTU) const
{
	outputTU.write(reinterpret_cast<const char*>(&notation), sizeof(notation));
	outputTU.write(reinterpret_cast<const char*>(&id), sizeof(id));
}


void DeallocationObject::read(std::ifstream& inputTU)
{
	inputTU.read(reinterpret_cast<char*>(&id), sizeof(id));
}


void DeallocationObject::get(std::array<char, fileLength>& buffer, const int offset)
{
	id = *reinterpret_cast<int*>(buffer.data() + offset);
}