#pragma once

#include "Camera.h"
#include "Compound.h"
#include <unordered_map>

namespace fl
{

class Hub;

enum class MachineType
{
	NONE,
	CONVEYOR,
	MINER,
	FURNACE,
	HUB_INPUT,
	HUB_BLANK
};

class Machine
{
public:

	Machine(const bool inputSides[], const bool outputSides[], const int& position, const uint8_t& machineSpeed, const uint8_t& storageCapacity, const TileType& tile, const sdl::SpriteEnum& machineSprite, const uint8_t& rotation, const MachineType& machineType);
	~Machine();

	static void SetMap(std::unordered_map<int, Machine*>* machineMap);

	virtual void Tick() {};
	
	void UpdateIO();
	virtual void ResetIO() {};
	void RemoveMachineFromIO(Machine* machine);

	void TransferItems();

	void Rotate();
	
	virtual void Turn() {};

	void AddToInputStorage(Compound* compound);
	void AddToOutputQueue(Compound* compound);

	const uint8_t GetStorageSize();

	void Output();

public:

	static std::unordered_map<int, Machine*>* machineMap;
	
	const int pos;

	MachineType type;

	const uint8_t rotation;
	const uint8_t storageCap;

	sdl::SpriteEnum sprite;
	uint8_t animationState;

	uint8_t speed;
protected:

	Machine* input[4];
	Machine* output[4];

	std::vector<Compound*> inputStorage;
	std::vector<Compound*> outputQueue;

	bool canInput[4];
	bool canOutput[4];

	TileType placedOn;

	friend class Hub;
};

}