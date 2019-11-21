#pragma once


/*
	Represents an element from the memory pool
	An element from the memory pool is composed by two things: 
	- a start address of allocated/unallocated memory
	- the size of the block of allocated/unallocated memory
*/
class PoolElement
{
public:
	PoolElement(char* address, size_t size) : address(address), size(size) {}
	void updateElement(char* address, size_t size);

	friend bool operator<(const PoolElement& a, const PoolElement& b);

public:
	char* address;	// start address of the block of memory (allocated/unallocated)
	size_t size;	// size of the block of memory (allocated/unallocated)
};