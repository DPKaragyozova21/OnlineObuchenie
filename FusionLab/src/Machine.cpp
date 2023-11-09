#include "Machine.h"

namespace fl
{

void Machine::Rotate()
{
	static bool tempIn, tempOut;
	tempIn = sideInput[0];
	tempOut = sideOutput[0];

	for (int i = 0; i < 3; i++)
	{
		sideInput[i] = sideInput[i + 1];
		sideOutput[i] = sideOutput[i + 1];
	}

	sideInput[3] = tempIn;
	sideOutput[3] = tempOut;

	if (rotation == 3) rotation = 0;
	else rotation++;
}

}