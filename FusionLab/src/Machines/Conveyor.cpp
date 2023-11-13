#include "Conveyor.h"

#include <iostream>

namespace fl
{

const bool inputSides[4] = { 0, 0, 0, 1 };
const bool outputSides[4] = { 1, 1, 1, 0 };

const int conveyorSpeed = 9;

Conveyor::Conveyor(const int& position, const TileType& tile, const uint8_t& rotation) :
	Machine(inputSides, outputSides, position, conveyorSpeed, tile, sdl::SpriteEnum::MACHINE_CONVEYOR, rotation, MachineType::CONVEYOR)
{
	Rotate();
	UpdateIO();

	Turn();
}

void Conveyor::ResetIO()
{
	for (int i = 0; i < 4; i++)
	{
		canInput[i] = inputSides[i];
		canOutput[i] = outputSides[i];
	}

	Rotate();
}

void Conveyor::Turn()
{
	for (int i = 0; i < 4; i++)
	{
		if (canInput[i])
		{
			if (output[(i + 2) % 4])
			{
				if (output[(i + 1) % 4])
				{
					output[(i + 1) % 4]->RemoveMachineFromIO(this);
					output[(i + 1) % 4] = nullptr;
					canOutput[(i + 3) % 4] = false;
				}
				if (output[(i + 3) % 4])
				{
					output[(i + 3) % 4]->RemoveMachineFromIO(this);
					output[(i + 3) % 4] = nullptr;
					canOutput[(i + 3) % 4] = false;
				}
				for (int j = 0; j < 4; j++) canOutput[j] = false;

				sprite = sdl::SpriteEnum::MACHINE_CONVEYOR;
			}
			else if (output[(i + 1) % 4])
			{
				if (output[(i + 3) % 4])
				{
					output[(i + 3) % 4]->RemoveMachineFromIO(this);
					output[(i + 3) % 4] = nullptr;
					canOutput[(i + 3) % 4] = false;
				}
				for (int j = 0; j < 4; j++) canOutput[j] = false;

				sprite = sdl::SpriteEnum::MACHINE_CONVEYOR_RIGHT;
			}
			else if (output[(i + 3) % 4])
			{
				for (int j = 0; j < 4; j++) canOutput[j] = false;

				sprite = sdl::SpriteEnum::MACHINE_CONVEYOR_LEFT;
			}
		}
	}
}

void Conveyor::Tick()
{
	static uint8_t outCount;

	if (!storage.empty()) std::cout << storage.back()->formula << " ";

	outCount = 0;
	for (int i = 0; i < 4; i++)
	{
		if (output[i]) outCount++;
		if (output[i] && !storage.empty())
		{
			output[i]->AddToTransferQueue(storage.back());
			storage.pop_back();
		}
	}

	if (outCount == 0)
	{
		for (int i = 0; i < 4; i++)
		{
			canInput[i] = inputSides[i];
			canOutput[i] = outputSides[i];
		}

		Rotate();

		sprite = sdl::SpriteEnum::MACHINE_CONVEYOR;
	}
}

}