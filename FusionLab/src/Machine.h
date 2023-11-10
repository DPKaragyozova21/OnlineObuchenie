#pragma once

#include "Camera.h"

#include <unordered_map>

namespace fl
{

class Machine
{
public:

	virtual void Tick() = 0;
	
	void UpdateIO();
	void RemoveMachineFromIO(Machine* machine);

	void TransferItems();

	void Rotate();

public:

	uint8_t rotation;

	static std::unordered_map<int, Machine*>& machineMap;

private:

	Machine* input[4];
	Machine* output[4];

	std::vector<int*> storage;
	std::vector<int*> transferStorage;

	bool canInput[4];
	bool canOutput[4];

	int pos;
};

}