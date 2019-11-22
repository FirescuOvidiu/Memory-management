#pragma once
#include "stdafx.h"


class Diagnostics
{
	Diagnostics();
	~Diagnostics();
private:
	std::ofstream m_diagFile;
	int totalMemory;
	int maxMemoryUtilization;
	double avgMemoryUtilization;

};