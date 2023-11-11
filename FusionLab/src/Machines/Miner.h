#pragma once

#include "Machine.h"

namespace fl
{

class Miner : Machine
{
public:

	Miner(const int& position, const TileType& tile);

	void Tick() override;
};
}