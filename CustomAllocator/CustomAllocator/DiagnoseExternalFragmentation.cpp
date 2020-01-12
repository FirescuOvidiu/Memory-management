#include "stdafx.h"


/*
	Method used to initialize data member totalMemory
*/
void DiagnoseExternalFragmentation::initExternalFrag(const int _totalMemory)
{
	totalMemory = _totalMemory;
}


/*
	Method used to update data members used to calculate the fragmentation
*/
void DiagnoseExternalFragmentation::updateExternalFrag(const int totalMemoryAvailable, const int biggestContMemory)
{
	avgFragmentation += 1 - (double)biggestContMemory / totalMemoryAvailable;
	nrFragmentation++;
}


/*
	Destructor used to write informations gathered during the application into the file diagExternalFrag.diag
*/
DiagnoseExternalFragmentation::~DiagnoseExternalFragmentation()
{
	diagFile.open("diagExternalFrag.diag", std::ofstream::out);

	avgFragmentation = avgFragmentation / nrFragmentation * 100;

	diagFile << std::setprecision(2) << std::fixed << "\n\n\t" << "<----------------------- START OF APPLICATION ----------------------->" << "\n\n";
	diagFile << "\tTotal memory allocated by the memory pool: " << totalMemory << " bytes.\n\n";
	diagFile << "\tThe average number of bytes wasted during the application was: " << avgFragmentation * totalMemory / 100 << " bytes.\n";
	diagFile << "\tFragmentation during the application was:" << avgFragmentation << "%.\n\n";
	diagFile << "\t" << "<----------------------- END OF APPLICATION ----------------------->";

	diagFile.close();
}