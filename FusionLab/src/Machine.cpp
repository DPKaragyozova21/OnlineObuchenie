#include "Machine.h"

#include <iostream>

namespace fl
{

std::unordered_map<int, Machine*>* Machine::machineMap;

Machine::Machine(const bool inputSides[], const bool outputSides[], const int& position, const uint8_t& machineSpeed, const uint8_t& storageCapacity, const TileType& tile, const sdl::SpriteEnum& machineSprite, const uint8_t& rotation, const MachineType& machineType) :
	pos(position),
	type(machineType),
	rotation(rotation),
	storageCap(storageCapacity),
	sprite(machineSprite),
	animationState(0),
	speed(machineSpeed),
	input{ nullptr, nullptr, nullptr, nullptr },
	output{ nullptr, nullptr, nullptr, nullptr },
	outputQueue(),
	inputStorage(),
	canInput{ inputSides[0], inputSides[1], inputSides[2], inputSides[3] },
	canOutput{ outputSides[0], outputSides[1], outputSides[2], outputSides[3] },
	placedOn(tile)
{
	Rotate();
	UpdateIO();
}

Machine::~Machine()
{
	for (auto i : outputQueue)
	{
		delete i;
	}
	
	for (auto i : inputStorage)
	{
		delete i;
	}
}

void Machine::SetMap(std::unordered_map<int, Machine*>* map)
{
	machineMap = map;
}

void Machine::UpdateIO()
{
	for (int i = 0; i < 4; i++)
	{
		output[i] = nullptr;
		input[i] = nullptr;
	}

	if (canInput[(int)Side::LEFT] && (machineMap[0].find(pos - 1) != machineMap[0].end()) && machineMap[0][pos - 1])
	{
		if (machineMap[0][pos - 1]->canOutput[(int)Side::RIGHT] && !machineMap[0][pos - 1]->output[(int)Side::RIGHT])
		{
			input[(int)Side::LEFT] = machineMap[0][pos - 1];
			machineMap[0][pos - 1]->output[(int)Side::RIGHT] = this;
			machineMap[0][pos - 1]->Turn();
		}
	}
	if (canInput[(int)Side::RIGHT] && (machineMap[0].find(pos + 1) != machineMap[0].end()) && machineMap[0][pos + 1])
	{
		if (machineMap[0][pos + 1]->canOutput[(int)Side::LEFT] && !machineMap[0][pos + 1]->output[(int)Side::LEFT])
		{
			input[(int)Side::RIGHT] = machineMap[0][pos + 1];
			machineMap[0][pos + 1]->output[(int)Side::LEFT] = this;
			machineMap[0][pos + 1]->Turn();
		}
	}
	if (canInput[(int)Side::UP] && (machineMap[0].find(pos - 1000) != machineMap[0].end()) && machineMap[0][pos - 1000])
	{
		if (machineMap[0][pos - 1000]->canOutput[(int)Side::DOWN] && !machineMap[0][pos - 1000]->output[(int)Side::DOWN])
		{
			input[(int)Side::UP] = machineMap[0][pos - 1000];
			machineMap[0][pos - 1000]->output[(int)Side::DOWN] = this;
			machineMap[0][pos - 1000]->Turn();
		}
	}
	if (canInput[(int)Side::DOWN] && (machineMap[0].find(pos + 1000) != machineMap[0].end()) && machineMap[0][pos + 1000])
	{
		if (machineMap[0][pos + 1000]->canOutput[(int)Side::UP] && !machineMap[0][pos + 1000]->output[(int)Side::UP])
		{
			input[(int)Side::DOWN] = machineMap[0][pos + 1000];
			machineMap[0][pos + 1000]->output[(int)Side::UP] = this;
			machineMap[0][pos + 1000]->Turn();
		}
	}

	if (canOutput[(int)Side::LEFT] && (machineMap[0].find(pos - 1) != machineMap[0].end()) && machineMap[0][pos - 1])
	{
		if (machineMap[0][pos - 1]->canInput[(int)Side::RIGHT] && !machineMap[0][pos - 1]->output[(int)Side::RIGHT])
		{
			output[(int)Side::LEFT] = machineMap[0][pos - 1];
			machineMap[0][pos - 1]->input[(int)Side::RIGHT] = this;
		}
	}
	if (canOutput[(int)Side::RIGHT] && (machineMap[0].find(pos + 1) != machineMap[0].end()) && machineMap[0][pos + 1])
	{
		if (machineMap[0][pos + 1]->canInput[(int)Side::LEFT] && !machineMap[0][pos + 1]->output[(int)Side::LEFT])
		{
			output[(int)Side::RIGHT] = machineMap[0][pos + 1];
			machineMap[0][pos + 1]->input[(int)Side::LEFT] = this;
		}
	}
	if (canOutput[(int)Side::UP] && (machineMap[0].find(pos - 1000) != machineMap[0].end()) && machineMap[0][pos - 1000])
	{
		if (machineMap[0][pos - 1000]->canInput[(int)Side::DOWN] && !machineMap[0][pos - 1000]->output[(int)Side::DOWN])
		{
			output[(int)Side::UP] = machineMap[0][pos - 1000];
			machineMap[0][pos - 1000]->input[(int)Side::DOWN] = this;
		}
	}
	if (canOutput[(int)Side::DOWN] && (machineMap[0].find(pos + 1000) != machineMap[0].end()) && machineMap[0][pos + 1000])
	{
		if (machineMap[0][pos + 1000]->canInput[(int)Side::UP] && !machineMap[0][pos + 1000]->output[(int)Side::UP])
		{
			output[(int)Side::DOWN] = machineMap[0][pos + 1000];
			machineMap[0][pos + 1000]->input[(int)Side::UP] = this;
		}
	}
}

void Machine::RemoveMachineFromIO(Machine* machine)
{
	for (int i = 0; i < 4; i++)
	{
		if (input[i] == machine) input[i] = nullptr;
		if (output[i] == machine) output[i] = nullptr;
	}
}

void Machine::TransferItems()
{
	for (Compound* i : inputStorage)
	{
		outputQueue.push_back(i);
	}
	
	inputStorage.clear();
}

void Machine::Rotate()
{
	static bool tempIn, tempOut;
	for (int i = 0; i < rotation; i++)
	{
		tempIn = canInput[3];
		tempOut = canOutput[3];

		for (int j = 3; j > 0; j--)
		{
			canInput[j] = canInput[j - 1];
			canOutput[j] = canOutput[j - 1];
		}

		canInput[0] = tempIn;
		canOutput[0] = tempOut;
	}
}

void Machine::AddToInputStorage(Compound* compound)
{
	if (outputQueue.size() < storageCap) inputStorage.push_back(compound);
}

void Machine::AddToOutputQueue(Compound* compound)
{
	if(outputQueue.size() < storageCap) outputQueue.push_back(compound);
}

const uint8_t Machine::GetStorageSize()
{
	return inputStorage.size();
}

void Machine::Output()
{
	for (int i = 0; i < 4; i++)
	{
		if (output[i] && !outputQueue.empty() && output[i]->GetStorageSize() < output[i]->storageCap)
		{
			output[i]->AddToInputStorage(outputQueue.back());
			outputQueue.pop_back();
		}
	}
}

}