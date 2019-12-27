#include "stdafx.h"


void DiagnoseExternalFragmentation::initExternalFrag(const int _totalMemory)
{
	totalMemory = _totalMemory;
}


void DiagnoseExternalFragmentation::updateExternalFrag(const int totalMemoryAvailable, const int biggestContMemory)
{
	double currFragmentation{ 0 };

	currFragmentation = 1 - (double)biggestContMemory / totalMemoryAvailable;
	avgFragmentation += currFragmentation;
	nrFragmentation++;
}


DiagnoseExternalFragmentation::~DiagnoseExternalFragmentation()
{
	avgFragmentation = avgFragmentation / nrFragmentation * 100;

	diagFile.open("diagExternalFrag.diag", std::ofstream::out);

	diagFile << std::setprecision(2) << std::fixed << "\n\n\t" << "<----------------------- START OF APPLICATION ----------------------->" << "\n\n";
	diagFile << "\tTotal memory allocated by the memory pool: " << totalMemory << " bytes.\n\n";
	diagFile << "\tThe average number of bytes wasted during the application was: " << avgFragmentation * totalMemory / 100 << " bytes.\n";
	diagFile << "\tFragmentation during the application was:" << avgFragmentation << "%.";
	diagFile << "\t" << "<----------------------- END OF APPLICATION ----------------------->";

	diagFile.close();
}