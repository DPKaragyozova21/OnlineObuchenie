#pragma once

#include "Machine.h"

namespace fl
{

class Conveyor : public Machine
{
public:

	Conveyor(const int& position, const TileType& tile, const uint8_t& rotation);

	void Tick() override;
};

}
