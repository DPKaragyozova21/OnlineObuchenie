#include "Miner.h"

#include <iostream>

namespace fl
{

const bool inputSides[4] = { 0, 0, 0, 0 };
const bool outputSides[4] = { 0, 1, 0, 0 };

const int speed = 9;

Miner::Miner(const int& position, const TileType& tile, const uint8_t& rotation) :
	Machine(inputSides, outputSides, position, speed, tile, sdl::SpriteEnum::MACHINE_MINER, rotation, MachineType::MINER)
{
	Rotate();
	UpdateIO();
}

void Miner::Tick()
{	
	switch (placedOn)
	{
	case TileType::O2:
		AddToTransferQueue(new int(2));
		break;

	case TileType::H2:
		AddToTransferQueue(new int(3));
		break;

	}

	for (int i = 0; i < 4; i++)
	{
		if (output[i] && !storage.empty())
		{
			output[i]->AddToTransferQueue(storage.back());
			storage.pop_back();
		}
	}
}

}