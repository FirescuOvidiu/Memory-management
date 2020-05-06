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
	if (totalTime != 0)
	{
		benchmarkFile.open("benchmark.txt", std::ofstream::out);

		benchmarkFile << std::setprecision(2) << std::fixed << "\n\n\t" << "<----------------------- BENCHMARKING ----------------------->" << "\n\n";
		benchmarkFile << "\tTotal time: " << totalTime << "\n\n";
		benchmarkFile << "\t" << "<----------------------- END ----------------------->";

		benchmarkFile.close();
	}
}