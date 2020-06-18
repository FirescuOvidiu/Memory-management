#pragma once


class DeallocationObject
{
public:
	void write(std::ofstream& outputTU) const;
	void read(std::ifstream& inputTU);
	void get(char buffer[], const int offset);

public:
	static const char notation;
	int id;
};