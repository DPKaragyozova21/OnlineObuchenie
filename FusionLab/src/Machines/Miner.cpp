#include "Miner.h"

#include <iostream>

namespace fl
{

const bool inputSides[4] = { 0, 0, 0, 0 };
const bool outputSides[4] = { 1, 0, 0, 0 };

Miner::Miner(const int& position, const TileType& tile, const uint8_t& rotation) :
	Machine(inputSides, outputSides, 6, position, tile, sdl::SpriteEnum::MACHINE_MINER, rotation)
{

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