#pragma once
#include "stdafx.h"


/*
	This class is used to evaluate the fragmentation state of the memory pool
	It also provides informations about the memory utilization
*/
class Diagnostics
{
	Diagnostics();
	~Diagnostics();

private:
	std::ofstream diagFile;
	int totalMemory;
	int maxMemoryUtilization;
	double avgMemoryUtilization;
	double avgMaxContinousMemory;

	// need to invent a metric to evaluate the fragmentation state of the memory pool
};