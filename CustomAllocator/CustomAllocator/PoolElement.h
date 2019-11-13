#pragma once

class PoolElement
{
public:
	PoolElement(char* adress, size_t size) : adress(adress), size(size) {}
	void updateElement(char* adress, size_t size);

	friend bool operator<(const PoolElement& a, const PoolElement& b);
public:
	char* adress;
	size_t size;
};