#include "stdafx.h"

GenerateTestUnits generatorTU(std::pair<int, int>(1, 400000), std::pair<int, int>(0, 20));

int main()
{
	// testA();
	// testB();
	// testC();
	// testD();
	generatorTU.generateTU();
	generatorTU.convertBinaryFile();
	generatorTU.loadTU();
	deserialization();
	evaluateFragmentationState();
}