#include "stdafx.h"

GenerateTestUnits generatorTU(4096, 200, std::pair<int, int>(32, 64), std::pair<int, int>(0, 10));

int main()
{
	// testA();
	// testB();
	// testC();
	//testD();
	generatorTU.generateTU();
	//generatorTU.convertBinaryFile();
	generatorTU.loadTU();
}