#pragma once


/*
	Represents an element from the memory pool
	An element from the memory pool is composed of: 
	- the start address of an allocated/unallocated memory block
	- the size of the allocated/unallocated memory block
*/
class PoolElement
{
public:
	PoolElement(char* address = {}, size_t size = {}) : address(address), size(size) {}
	bool operator<(const PoolElement& poolElement) const;
	friend std::ostream& operator<<(std::ostream& output, const PoolElement& poolElement);
	friend std::istream& operator>>(std::istream& input, PoolElement& poolElement);

	void updateElement(char* _address, size_t _size);
	static void setStartAddress(char* startAddresss);

public:
	char* address;	// Start address of the block of memory (allocated/unallocated)
	size_t size;	// Size of the block of memory (allocated/unallocated)

private:
	static char* startAddress;
};