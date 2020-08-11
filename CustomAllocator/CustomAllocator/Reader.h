#pragma once


class Reader
{
public:
	static std::list<PoolElement> readList(std::ifstream& input);
	static std::set<PoolElement> readSet(std::ifstream& input);
	static std::vector<std::set<PoolElement>> readVectorOfSets(std::ifstream& input);

	template <typename T>
	static T readVariable(std::ifstream& input);
};



template<typename T>
inline T Reader::readVariable(std::ifstream& input)
{
	T variable;

	input.read(reinterpret_cast<char*>(&variable), sizeof(variable));

	return variable;
}