#include "Furnace.h"

#include <iostream>

namespace fl
{

const bool inputSides[4] = { 1, 0, 1, 0 };
const bool outputSides[4] = { 0, 0, 0, 1 };

const uint8_t furnaceSpeed = 29;
const uint8_t furnaceCap = 6;


Furnace::Furnace(const int& position, const TileType& tile, const uint8_t& rotation) :
	Machine(inputSides, outputSides, position, furnaceSpeed, furnaceCap, tile, sdl::SpriteEnum::MACHINE_FURNACE, rotation, MachineType::FURNACE)
{
	
}

void Furnace::Tick()
{
	/*for (Compound* i : storage)
	{
		std::cout << i->formula << " ";
	}*/

	if (storage.empty()) animationState = 0;
	else
	{
		animationState++;
		if (animationState == 4) animationState = 1;
	}

	if (storage.size() >= storageCap) storage.clear();
}

}