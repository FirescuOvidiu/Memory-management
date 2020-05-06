#pragma once


/*
	Used for choosing a custom allocator based on a context
*/
class MemoryManagement
{
public:
	MemoryManagement(const int context = 0, const int poolSize = 0, const diagnosticTypes diagType = diagnosticTypes::No_Diagnostic, const benchMarkingTypes benchmarkType = benchMarkingTypes::No_BenchMark);
	friend std::ostream& operator<<(std::ostream& output, const MemoryManagement& memoryManagement);
	friend std::istream& operator>>(std::istream& input, MemoryManagement& memoryManagement);

	void* __cdecl allocMemory(size_t aSize, int aBlockUse, char const* aFileName, int aLineNumber);
	void __cdecl freeMemory(void* aBlock, int aBlockUse);

	void evaluateFragmentation();

	~MemoryManagement();

private:
	Strategy* customAllocator;
	DiagnosticTools* diagTools;
	diagnosticTypes diagType;
	BenchMarking timer;
	benchMarkingTypes benchmarkType;
	int context, poolSize;
};