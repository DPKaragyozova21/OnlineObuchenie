#pragma once

#include <vector>
#include <cstdint>

namespace fl
{

class Machine
{
public:

	virtual void Tick() = 0;
	virtual void TransferItems() = 0;

	virtual void UpdateIO() = 0;

	void Rotate();

public:

	uint8_t rotation;

public:

	std::vector<Machine*> input;
	std::vector<Machine*> output;

	std::vector<int*> storage;
	std::vector<int*> transferStorage;

	bool sideInput[4];
	bool sideOutput[4];
};

}