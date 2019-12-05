#pragma once
#include "stdafx.h"


/*
	This class is used to evaluate the fragmentation state of the memory pool
	It also provides informations about the memory utilization
*/
class Diagnostics
{
public:
	Diagnostics() : totalMemory(0), maxMemoryUtilization(0), nrMemoryUtilization(0), avgMemoryUtilization(0), nrMaxContinuousMemory(0), avgMaxContinuousMemory(0),
		segmentLength(0), numberSegments(0), maxNumberSegments(0), nrSegments(0), avgSegments(0), avgMemoryLost(0), maxMemoryLost(0) {}

	void initializeDiagnostics(const int _totalMemory);
	void updateMemoryInf(const int memoryUtilization, const int maxContinuousMemory);
	void updateSegmentInf(const std::list<PoolElement>& mAvailable);

	int getTotalMemory() const;

	~Diagnostics();

private:
	// Evaluates the state of the memory, contains informations about memory 
	std::ofstream diagFile;
	int totalMemory;
	int maxMemoryUtilization;
	int nrMemoryUtilization;			// Used to calculate the average memory utilization
	int nrMaxContinuousMemory;		   // Used to calculate the average of the maximum continuous memory
	double avgMemoryUtilization;
	double avgMaxContinuousMemory;
	

	// Evaluates the state of the segments, contains informations about segments
	// A segment represent a continuous block of memory which is too small to be used in allocations
	int segmentLength;			// Represents the lenght of a segment that will be considered too small to be used
	int numberSegments;			// Represents the total number of segments that will have during the application
	int maxNumberSegments;		// Represents the maximum number of segments that will have during the application
	int maxMemoryLost;
	int nrSegments;				// Used to calculate the average number of segments
	double avgSegments;			// Represents average number of segments that will have during the program 
	double avgMemoryLost;

	/*
		Example: If during a application we have 0 segments, than 1 segment(after allocation), than 2 segments(after allocation), than 0 segments(after deallocation).
		Than numberSegments			= 0 + 1 + 2 + 0 = 3
			 maxNumberSegments		= 2
			 avgSegments			= (0 + 1 + 2 + 0) / 4 = 3/4 = 0.75
		
		Conclusion:
		That means we had in total 3 segments of memory that weren't used during the application.
		The maximum number of segments at a time was 2.
		On every allocation around 0.75 of the segmentLength of memory was wasted because couldn't be used.
	*/
};
