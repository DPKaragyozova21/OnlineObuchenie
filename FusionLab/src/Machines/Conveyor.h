#pragma once

#include "Machine.h"

namespace fl
{

class Conveyor : public Machine
{
public:

	Conveyor(const int& position, const TileType& tile, const uint8_t& rotation);

	void ResetIO() override;
	void Turn() override;

	void Tick() override;
};

}
