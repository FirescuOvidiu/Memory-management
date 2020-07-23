#pragma once


/*
	Represents an element from the memory pool
	An element from the memory pool is composed of: 
	- the start address of the memory block
	- the size of the  memory block
*/
class PoolElement
{
public:
	PoolElement(char* address = {}, const size_t size = {}) : address(address), size(size) {}
	bool operator<(const PoolElement& block) const;

	friend std::ofstream& operator<<(std::ofstream& output, const PoolElement& block);
	friend std::ifstream& operator>>(std::ifstream& input, PoolElement& block);

	void updateElement(char* _address, const size_t _size);
	static void setStartAddress(char* _startAddress);

public:
	char* address;	// Address of the block of memory
	size_t size;	// Size of the block of memory

private:
	static char* startAddress;	// Start address of the memory pool
};