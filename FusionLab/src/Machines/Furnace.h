#pragma once

#include "Machine.h"

namespace fl
{

class Furnace : public Machine
{
	Furnace(const int& position, const TileType& tile, const uint8_t& rotation);

	void Tick() override;
};

}