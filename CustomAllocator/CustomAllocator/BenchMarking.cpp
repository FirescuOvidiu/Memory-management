#include "stdafx.h"

/*
	Method used to start the timer
*/
void BenchMarking::startTimer()
{
	// Keep track of the time point that we get from the high resolution clock
	startTimePoint = std::chrono::high_resolution_clock::now();
}


/*
	Method used to stop the timer
*/
void BenchMarking::stopTimer()
{
	// Get the current time point 
	auto endTimePoint = std::chrono::high_resolution_clock::now();

	// Transform the time points into microseconds
	auto start = std::chrono::time_point_cast<std::chrono::microseconds>(startTimePoint).time_since_epoch().count();
	auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimePoint).time_since_epoch().count();

	totalTime += end - start;
}


/*
	Destructor used to write into a file the results of the benchmarking
*/
BenchMarking::~BenchMarking()
{
	if (totalTime != 0)
	{
		benchmarkFile.open("benchmark.txt", std::ofstream::out);

		benchmarkFile << std::setprecision(2) << std::fixed << "\n\n\t" << "<----------------------- BENCHMARKING ----------------------->" << "\n\n";
		benchmarkFile << "\tTotal time: " << totalTime << " microseconds (" << totalTime * 0.001 << " milliseconds).\n\n";
		benchmarkFile << "\t" << "<----------------------- END ----------------------->";

		benchmarkFile.close();
	}
}