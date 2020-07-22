#pragma once


class Writer
{
public:
	static void writeList(std::ofstream& output, std::list<PoolElement> vector);
	static void writeSet(std::ofstream& output, std::set<PoolElement> set);
	static void writeSortedLists(std::ofstream& output, const std::list<PoolElement>& first, const std::list<PoolElement>& second);

	template <typename T>
	static void writeVariable(std::ofstream& output, T variable);
};


template<typename T>
inline void Writer::writeVariable(std::ofstream& output, T variable)
{
	output.write(reinterpret_cast<const char*>(&variable), sizeof(variable));
}