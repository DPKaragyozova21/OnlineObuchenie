#pragma once

#include "Machine.h"

namespace fl
{

class Miner : public Machine
{
public:

	Miner(const int& position, const TileType& tile, const uint8_t& rotation);

	void Tick() override;
};
}