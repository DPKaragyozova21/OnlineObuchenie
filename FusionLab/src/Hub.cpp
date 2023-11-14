#include "Hub.h"

#include <iostream>

namespace fl
{

Hub::Hub() :
	parts()
{
	bool inputSides[4] = { 1, 1, 0, 0 };
	bool outputSides[4] = { 0, 0, 0, 0 };

	parts[0] = new Machine(inputSides, outputSides, 499499, 0, 10, TileType::NONE, sdl::SpriteEnum::HUB_CORNER, 0, MachineType::HUB_INPUT);
	parts[3] = new Machine(inputSides, outputSides, 499502, 0, 10, TileType::NONE, sdl::SpriteEnum::HUB_CORNER, 1, MachineType::HUB_INPUT);
	parts[12] = new Machine(inputSides, outputSides, 502499, 0, 10, TileType::NONE, sdl::SpriteEnum::HUB_CORNER, 3, MachineType::HUB_INPUT);
	parts[15] = new Machine(inputSides, outputSides, 502502, 0, 10, TileType::NONE, sdl::SpriteEnum::HUB_CORNER, 2, MachineType::HUB_INPUT);

	inputSides[0] = 0;

	parts[1] = new Machine(inputSides, outputSides, 499500, 0, 10, TileType::NONE, sdl::SpriteEnum::HUB_SIDE, 0, MachineType::HUB_INPUT);
	parts[2] = new Machine(inputSides, outputSides, 499501, 0, 10, TileType::NONE, sdl::SpriteEnum::HUB_SIDE, 0, MachineType::HUB_INPUT);
	parts[4] = new Machine(inputSides, outputSides, 500499, 0, 10, TileType::NONE, sdl::SpriteEnum::HUB_SIDE, 3, MachineType::HUB_INPUT);
	parts[7] = new Machine(inputSides, outputSides, 500502, 0, 10, TileType::NONE, sdl::SpriteEnum::HUB_SIDE, 1, MachineType::HUB_INPUT);
	parts[8] = new Machine(inputSides, outputSides, 501499, 0, 10, TileType::NONE, sdl::SpriteEnum::HUB_SIDE, 3, MachineType::HUB_INPUT);
	parts[11] = new Machine(inputSides, outputSides, 501502, 0, 10, TileType::NONE, sdl::SpriteEnum::HUB_SIDE, 1, MachineType::HUB_INPUT);
	parts[13] = new Machine(inputSides, outputSides, 502500, 0, 10, TileType::NONE, sdl::SpriteEnum::HUB_SIDE, 2, MachineType::HUB_INPUT);
	parts[14] = new Machine(inputSides, outputSides, 502501, 0, 10, TileType::NONE, sdl::SpriteEnum::HUB_SIDE, 2, MachineType::HUB_INPUT);

	inputSides[1] = 0;

	parts[5] = new Machine(inputSides, outputSides, 500500, 0, 10, TileType::NONE, sdl::SpriteEnum::HUB_INSIDE, 0, MachineType::HUB_BLANK);
	parts[6] = new Machine(inputSides, outputSides, 500501, 0, 10, TileType::NONE, sdl::SpriteEnum::HUB_INSIDE, 0, MachineType::HUB_BLANK);
	parts[9] = new Machine(inputSides, outputSides, 501500, 0, 10, TileType::NONE, sdl::SpriteEnum::HUB_INSIDE, 0, MachineType::HUB_BLANK);
	parts[10] = new Machine(inputSides, outputSides, 501501, 0, 10, TileType::NONE, sdl::SpriteEnum::HUB_INSIDE, 0, MachineType::HUB_BLANK);
}

void Hub::TransferItemsToHub()
{
	for (int i = 0; i < 16; i++)
	{
		for (auto i : parts[i]->storage)
		{
			if(hubStorage.find(i->formula) == hubStorage.end()) hubStorage[i->formula] = 0;
			hubStorage[i->formula]++;

			delete i;
		}

		parts[i]->storage.clear();
	}

	for (auto i : hubStorage)
	{
		std::cout << i.first << " " << i.second << std::endl;
	}
}

}