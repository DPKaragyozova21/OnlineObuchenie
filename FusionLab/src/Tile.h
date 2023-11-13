#pragma once

#include <stdint.h>

namespace fl
{

enum class TileType : uint8_t
{
    NONE,
    H2,
    O2
};

struct Tile
{
    TileType type;
};

}