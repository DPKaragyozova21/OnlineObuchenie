#include "Conveyor.h"

#include <iostream>

namespace fl
{

const bool inputSides[4] = { 0, 0, 0, 1 };
const bool outputSides[4] = { 1, 1, 1, 0 };

Conveyor::Conveyor(const int& position, const TileType& tile, const uint8_t& rotation) :
	Machine(inputSides, outputSides, position, 9, tile, sdl::SpriteEnum::MACHINE_CONVEYOR, rotation)
{
	Rotate();
	UpdateIO();

	std::cout << std::endl << position;
}

void Conveyor::Tick()
{
	if (!storage.empty()) std::cout << "e";
}

}