#include "Furnace.h"

namespace fl
{

const bool inputSides[4] = { 0, 0, 0, 0 };
const bool outputSides[4] = { 0, 1, 0, 0 };

const uint8_t furnaceSpeed = 1;
const uint8_t furnaceCap = 6;


Furnace::Furnace(const int& position, const TileType& tile, const uint8_t& rotation) :
	Machine(inputSides, outputSides, position, furnaceSpeed, furnaceCap, tile, sdl::SpriteEnum::MACHINE_CONVEYOR, rotation, MachineType::CONVEYOR)
{
	
}

void Furnace::Tick()
{
	
}

}