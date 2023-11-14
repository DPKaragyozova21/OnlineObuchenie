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
	for (auto i : inputStorage)
	{
		if (i->formula == "CO")
		{
			for (auto j : inputStorage)
			{
				if (j->formula == "Fe2O3")
				{
					AddToOutputQueue(new Compound("Fe"));
					AddToOutputQueue(new Compound("Fe"));

					delete j;
					delete i;
					break;
				}
				if (j->formula == "Fe3O4")
				{
					AddToOutputQueue(new Compound("Fe"));
					AddToOutputQueue(new Compound("Fe"));
					AddToOutputQueue(new Compound("Fe"));

					delete j;
					delete i;
					break;
				}
			}
			break;
		}
	}

	if (inputStorage.empty())
	{
		animationState = 0;
	}
	else
	{
		for (auto i : outputQueue)
		{
			std::cout << i->formula << " ";
		}

		animationState++;
		if (animationState == 4) animationState = 1;
	}

	if (inputStorage.size() >= storageCap) inputStorage.clear();
}

}