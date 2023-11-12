#include "Miner.h"

#include <iostream>

namespace fl
{

const bool inputSides[4] = { 0, 0, 0, 0 };
const bool outputSides[4] = { 0, 1, 0, 0 };

Miner::Miner(const int& position, const TileType& tile, const uint8_t& rotation) :
	Machine(inputSides, outputSides, position, 9, tile, sdl::SpriteEnum::MACHINE_MINER, rotation)
{
	Rotate();
	UpdateIO();
}

void Miner::Tick()
{	
	switch (placedOn)
	{
	case TileType::O2:
		break;
	}
}

}