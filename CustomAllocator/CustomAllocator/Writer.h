#pragma once


class Writer
{
public:
	template <typename T>
	static void writeVariable(std::ostream& output, T variable);
};


template<typename T>
inline void Writer::writeVariable(std::ostream& output, T variable)
{
	output.write(reinterpret_cast<const char*>(&variable), sizeof(variable));
}