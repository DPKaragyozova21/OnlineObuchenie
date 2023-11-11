#pragma once

#include "Camera.h"

#include <unordered_map>

namespace fl
{

enum class MachineType
{
	NONE,
	COVEYOR,
	MINER,
	SPLITTER
};

class Machine
{
public:

	Machine(const bool inputSides[], const bool outputSides[], const int& position, const uint8_t& machineSpeed, const TileType& tile, const sdl::SpriteEnum& machineSprite);
	static void SetMap(std::unordered_map<int, Machine*>* machineMap);

	virtual void Tick() = 0;
	
	void UpdateIO();
	void RemoveMachineFromIO(Machine* machine);

	void TransferItems();

	void Rotate();

public:

	uint8_t rotation;

	static std::unordered_map<int, Machine*>* machineMap;

	sdl::SpriteEnum sprite;
protected:

	Machine* input[4];
	Machine* output[4];

	std::vector<int*> storage;
	std::vector<int*> transferStorage;

	bool canInput[4];
	bool canOutput[4];

	int pos;
	uint8_t speed;
	TileType placedOn;

};

}