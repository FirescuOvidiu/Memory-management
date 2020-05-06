#pragma once


class BenchMarking
{
public:
	BenchMarking() : totalTime(0) {}

	void startTimer();
	void stopTimer();

	~BenchMarking();

private:
	std::chrono::time_point<std::chrono::high_resolution_clock> startTimePoint;
	long long totalTime;
};