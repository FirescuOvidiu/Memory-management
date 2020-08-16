#pragma once


typedef struct DeallocationObject
{
	void write(std::ofstream& outputTU) const;
	void read(std::ifstream& inputTU);
	void get(std::array<char, fileLength>& buffer, const int offset);

	static const char notation;
	int id;
}DeallocationObject;
