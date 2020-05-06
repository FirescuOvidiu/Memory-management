#include "stdafx.h"


void BenchMarking::startTimer()
{
	startTimePoint = std::chrono::high_resolution_clock::now();
}


void BenchMarking::stopTimer()
{
	auto endTimePoint = std::chrono::high_resolution_clock::now();

	auto start = std::chrono::time_point_cast<std::chrono::milliseconds>(startTimePoint).time_since_epoch();
	auto end = std::chrono::time_point_cast<std::chrono::milliseconds>(endTimePoint).time_since_epoch();

	totalTime += std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
}


BenchMarking::~BenchMarking()
{
	std::cout << "test" << totalTime << " ";
}