#include "stdafx.h"


Diagnostics::Diagnostics() : totalMemory(0), maxMemoryUtilization(0), avgMemoryUtilization(0), avgMaxContinousMemory(0)
{
	diagFile.open("DiagnosticsFile.diag", std::ofstream::out);
}


Diagnostics::~Diagnostics()
{
	diagFile.close();
}