#pragma once


typedef struct AllocationObject
{
	AllocationObject(int id = 0, int size = 0) : id(id), size(size) {}

	void write(std::ofstream& outputTU) const;
	void read(std::ifstream& inputTU);
	void get(std::array<char, fileLength>& buffer, int& offset);

	static const char notation;
	int id;
	int size;
}AllocationObject;