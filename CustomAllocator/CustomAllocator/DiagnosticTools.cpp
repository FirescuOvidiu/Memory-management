#include "stdafx.h"


/*
	Method used to display the fragmentation state of the custom allocators
	after we calculate it
*/
void DiagnosticTools::showFragmentationState(double fragmentationState)
{
	switch (diagType)
	{
	case diagnosticType::File_Diagnostic:
		diagFile.open("diagInternalFrag.diag", std::ofstream::out);

		diagFile << std::setprecision(2) << std::fixed << "\n\n\t" << "<----------------------- EVALUATE FRAGMENTATION ----------------------->" << "\n\n";
		diagFile << "\tTotal memory allocated by the memory pool: " << totalMemory << " bytes.\n\n";
		diagFile << "\tThe number of bytes wasted at this point is: " << fragmentationState << " bytes.\n";
		diagFile << "\tFragmentation at this point is:" << (fragmentationState * 100) / totalMemory << "%.\n\n";
		diagFile << "\t" << "<----------------------- END ----------------------->";

		diagFile.close();
		break;

	case diagnosticType::Console_Diagnostic:
		std::cout << std::setprecision(2) << std::fixed << "\n\n\t" << "<----------------------- EVALUATE FRAGMENTATION ----------------------->" << "\n\n";
		std::cout << "\tTotal memory allocated by the memory pool: " << totalMemory << " bytes.\n\n";
		std::cout << "\tThe number of bytes wasted at this point is: " << fragmentationState << " bytes.\n";
		std::cout << "\tFragmentation at this point is:" << (fragmentationState * 100) / totalMemory << "%.\n\n";
		std::cout << "\t" << "<----------------------- END ----------------------->";
		break;
	}
}