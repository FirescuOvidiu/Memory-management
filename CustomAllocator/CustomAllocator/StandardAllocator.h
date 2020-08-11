#pragma once
#include "stdafx.h"


class StandardAllocator final : public Strategy
{
public:

	std::ofstream& write(std::ofstream& output) const override;
	std::ifstream& read(std::ifstream& input) override;

	void* __cdecl allocMemory(size_t aSize, int aBlockUse, char const* aFileName, int aLineNumber) override;
	void __cdecl freeMemory(void* aBlock, int aBlockUse) override;
};