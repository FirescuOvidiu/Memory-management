#pragma once


typedef enum class BenchMarking_Types
{
	No_BenchMark = 1,
	Allocation_BenchMark = 2,
	Deallocation_BenchMark = 3,
	AllocationAndDeallocation_BenchMark = 4
}benchMarkingType;


class BenchMarking
{
public:
	BenchMarking() : totalTime(0) {}

	void startTimer();
	void stopTimer();

	~BenchMarking();

private:
	std::ofstream benchmarkFile;
	std::chrono::time_point<std::chrono::high_resolution_clock> startTimePoint;
	long long totalTime;
};