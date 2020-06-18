#pragma once


class AllocationObject
{
public:
	AllocationObject(int id = 0, int size = 0) : id(id), size(size) {}

	void write(std::ofstream& outputTU) const;
	void read(std::ifstream& inputTU);
	void get(char buffer[], int& offset);

public:
	static const char notation;
	int id;
	int size;
};