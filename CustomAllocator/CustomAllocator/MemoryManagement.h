#pragma once


/*
	Contains an allocator used to manage the allocations and deallocations, diagnostic tools used to evaluate the allocator, and operator<</operator>> for serialization/deserializaton
*/
class MemoryManagement final
{
public:
	MemoryManagement(const strategyType allocatorType = strategyType::StandardAllocator, const int poolSize = 1000000, const diagnosticType diagType = diagnosticType::No_Diagnostic, const LogType logType = LogType::No_Log);

	friend std::ofstream& operator<<(std::ofstream& output, const MemoryManagement& memoryManager);
	friend std::ifstream& operator>>(std::ifstream& input, MemoryManagement& memoryManager);

	void* __cdecl allocMemory(size_t aSize, int aBlockUse, char const* aFileName, int aLineNumber);
	void __cdecl freeMemory(void* aBlock, int aBlockUse);

	void evaluateFragmentation();

private:
	void setAllocatorAndDiagnostic(const strategyType _allocatorType);

private:
	std::unique_ptr<Strategy> allocator;
	std::unique_ptr<DiagnosticTools> diagTools;
	strategyType allocatorType;
	diagnosticType diagType;
	LogType logType;
	int poolSize;
};