#pragma once


class Reader
{
public:
	template <typename T>
	static T readVariable(std::istream& input);
};



template<typename T>
inline T Reader::readVariable(std::istream& input)
{
	T variable;

	input.read(reinterpret_cast<char*>(&variable), sizeof(variable));

	return variable;
}