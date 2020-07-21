#pragma once


/*
	Used for choosing a custom allocator based on a context
*/
class MemoryManagement
{
public:
	MemoryManagement(const strategyType context = strategyType::WorstFit, const int poolSize = 0, const diagnosticType diagType = diagnosticType::No_Diagnostic);
	friend std::ostream& operator<<(std::ostream& output, const MemoryManagement& memoryManagement);
	friend std::istream& operator>>(std::istream& input, MemoryManagement& memoryManagement);

	void* __cdecl allocMemory(size_t aSize, int aBlockUse, char const* aFileName, int aLineNumber);
	void __cdecl freeMemory(void* aBlock, int aBlockUse);

	void evaluateFragmentation();

private:
	std::unique_ptr<Strategy> customAllocator;
	strategyType context;
	std::unique_ptr<DiagnosticTools> diagTools;
	diagnosticType diagType;
	int poolSize;
};