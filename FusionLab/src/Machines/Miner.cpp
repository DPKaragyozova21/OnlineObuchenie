#include "Miner.h"

#include <iostream>

namespace fl
{

const bool inputSides[4] = { 0, 0, 0, 0 };
const bool outputSides[4] = { 0, 1, 0, 0 };

const uint8_t minerSpeed = 29;
const uint8_t minerCap = 10;

Miner::Miner(const int& position, const TileType& tile, const uint8_t& rotation) :
	Machine(inputSides, outputSides, position, minerSpeed, minerCap, tile, sdl::SpriteEnum::MACHINE_MINER, rotation, MachineType::MINER)
{

}

void Miner::Tick()
{	
	switch (placedOn)
	{
	case TileType::MAGNETITE:
		AddToInputStorage(new Compound("Fe3O4"));
		break;

	case TileType::HEMATITE:
		AddToInputStorage(new Compound("Fe2O3"));
		break;

	case TileType::CARBON:
		AddToInputStorage(new Compound("CO"));
		break;

	}

}

}