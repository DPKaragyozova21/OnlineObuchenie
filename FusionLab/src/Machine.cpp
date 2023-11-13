#include "Machine.h"

#include <iostream>

namespace fl
{

std::unordered_map<int, Machine*>* Machine::machineMap;

Machine::Machine(const bool inputSides[], const bool outputSides[], const int& position, const uint8_t& machineSpeed, const TileType& tile, const sdl::SpriteEnum& machineSprite, const uint8_t& rotation) :
	rotation(rotation),
	sprite(machineSprite),
	speed(machineSpeed),
	input{ nullptr, nullptr, nullptr, nullptr },
	output{ nullptr, nullptr, nullptr, nullptr },
	storage(),
	transferStorage(),
	canInput{ inputSides[0], inputSides[1], inputSides[2], inputSides[3] },
	canOutput{ outputSides[0], outputSides[1], outputSides[2], outputSides[3] },
	pos(position),
	placedOn(tile)
{

}

Machine::~Machine()
{
	for (auto i : storage)
	{
		delete i;
	}
	
	for (auto i : transferStorage)
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

	std::cout << canInput[(int)Side::LEFT] << " " << (machineMap[0].find(pos - 1) != machineMap[0].end()) << std::endl;
	if (canInput[(int)Side::LEFT] && (machineMap[0].find(pos - 1) != machineMap[0].end()) && machineMap[0][pos - 1])
	{
		std::cout << "inputLeft";
		if (machineMap[0][pos - 1]->canOutput[(int)Side::RIGHT] && !machineMap[0][pos - 1]->output[(int)Side::RIGHT])
		{
			input[(int)Side::LEFT] = machineMap[0][pos - 1];
			machineMap[0][pos - 1]->output[(int)Side::RIGHT] = this;
		}
	}
	std::cout << canInput[(int)Side::RIGHT] << " " << (machineMap[0].find(pos + 1) != machineMap[0].end()) << std::endl;
	if (canInput[(int)Side::RIGHT] && (machineMap[0].find(pos + 1) != machineMap[0].end()) && machineMap[0][pos + 1])
	{
		std::cout << "inputRight";
		if (machineMap[0][pos + 1]->canOutput[(int)Side::LEFT] && !machineMap[0][pos + 1]->output[(int)Side::LEFT])
		{
			input[(int)Side::RIGHT] = machineMap[0][pos + 1];
			machineMap[0][pos + 1]->output[(int)Side::LEFT] = this;
		}
	}
	std::cout << canInput[(int)Side::UP] << " " << (machineMap[0].find(pos - 10000) != machineMap[0].end()) << std::endl;
	if (canInput[(int)Side::UP] && (machineMap[0].find(pos - 10000) != machineMap[0].end()) && machineMap[0][pos - 10000])
	{
		std::cout << "inputUp";
		if (machineMap[0][pos - 10000]->canOutput[(int)Side::DOWN] && !machineMap[0][pos - 10000]->output[(int)Side::DOWN])
		{
			input[(int)Side::UP] = machineMap[0][pos - 10000];
			machineMap[0][pos - 10000]->output[(int)Side::DOWN] = this;
		}
	}
	std::cout << canInput[(int)Side::DOWN] << " " << (machineMap[0].find(pos + 10000) != machineMap[0].end()) << std::endl;
	if (canInput[(int)Side::DOWN] && (machineMap[0].find(pos + 10000) != machineMap[0].end()) && machineMap[0][pos + 10000])
	{
		std::cout << "inputDown";
		if (machineMap[0][pos + 10000]->canOutput[(int)Side::UP] && !machineMap[0][pos + 10000]->output[(int)Side::UP])
		{
			input[(int)Side::DOWN] = machineMap[0][pos + 10000];
			machineMap[0][pos + 10000]->output[(int)Side::UP] = this;
		}
	}

	std::cout << canOutput[(int)Side::LEFT] << " " << (machineMap[0].find(pos - 1) != machineMap[0].end()) << std::endl;
	if (canOutput[(int)Side::LEFT] && (machineMap[0].find(pos - 1) != machineMap[0].end()) && machineMap[0][pos - 1])
	{
		if (machineMap[0][pos - 1]->canInput[(int)Side::RIGHT] && !machineMap[0][pos - 1]->output[(int)Side::RIGHT])
		{
			output[(int)Side::LEFT] = machineMap[0][pos - 1];
			machineMap[0][pos - 1]->output[(int)Side::RIGHT] = this;
		}
	}
	std::cout << canOutput[(int)Side::RIGHT] << " " << (machineMap[0].find(pos + 1) != machineMap[0].end()) << std::endl;
	if (canOutput[(int)Side::RIGHT] && (machineMap[0].find(pos + 1) != machineMap[0].end()) && machineMap[0][pos + 1])
	{
		std::cout << "outputRight";
		if (machineMap[0][pos + 1]->canInput[(int)Side::LEFT] && !machineMap[0][pos + 1]->output[(int)Side::LEFT])
		{
			output[(int)Side::RIGHT] = machineMap[0][pos + 1];
			machineMap[0][pos + 1]->output[(int)Side::LEFT] = this;
		}
	}
	std::cout << canOutput[(int)Side::UP] << " " << (machineMap[0].find(pos - 10000) != machineMap[0].end()) << std::endl;
	if (canOutput[(int)Side::UP] && (machineMap[0].find(pos - 10000) != machineMap[0].end()) && machineMap[0][pos - 10000])
	{
		std::cout << "outputUp";
		if (machineMap[0][pos - 10000]->canInput[(int)Side::DOWN] && !machineMap[0][pos - 10000]->output[(int)Side::DOWN])
		{
			output[(int)Side::UP] = machineMap[0][pos - 10000];
			machineMap[0][pos - 10000]->output[(int)Side::DOWN] = this;
		}
	}
	std::cout << canOutput[(int)Side::DOWN] << " " << (machineMap[0].find(pos + 10000) != machineMap[0].end()) << std::endl;
	if (canOutput[(int)Side::DOWN] && (machineMap[0].find(pos + 10000) != machineMap[0].end()) && machineMap[0][pos + 10000])
	{
		std::cout << "outputDown";
		if (machineMap[0][pos + 10000]->canInput[(int)Side::UP] && !machineMap[0][pos + 10000]->output[(int)Side::UP])
		{
			output[(int)Side::DOWN] = machineMap[0][pos + 10000];
			machineMap[0][pos + 10000]->output[(int)Side::UP] = this;
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
	for (int* i : transferStorage)
	{
		storage.push_back(i);
	}
	
	transferStorage.clear();
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

void Machine::AddToTransferQueue(int* compound)
{
	transferStorage.push_back(compound);
}

}