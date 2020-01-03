#pragma once


class BuddySystem : public Strategy, public DiagnosticTools
{
public:
	BuddySystem(size_t poolSize);

	void* __cdecl allocMemory(size_t aSize, int aBlockUse, char const* /*aFileName*/, int /*aLineNumber*/) override;
	void __cdecl freeMemory(void* aBlock, int /*aBlockUse*/) override;

	~BuddySystem();
};