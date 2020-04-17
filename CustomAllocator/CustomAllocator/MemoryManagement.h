#pragma once


/*
	Used for choosing a custom allocator based on a context
*/
class MemoryManagement
{
public:
	MemoryManagement(const int context = 0, const int poolSize = 0, const diagnosticTypes diagType = diagnosticTypes::No_Diagnostic);

	void* __cdecl allocMemory(size_t aSize, int aBlockUse, char const* aFileName, int aLineNumber);
	void __cdecl freeMemory(void* aBlock, int aBlockUse);

	void evaluateFragmentation();

	void serializationMemoryManagement(std::ofstream& output);
	void deserializationMemoryManagement(std::ifstream& input);

	~MemoryManagement();

private:
	Strategy* customAllocator;
	DiagnosticTools* diagTools;
	diagnosticTypes diagType;
	int context, poolSize;
};