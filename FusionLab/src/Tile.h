#pragma once

#include <stdint.h>

namespace fl
{

enum class TileType : uint32_t
{
    NONE,
    O2
};

struct Tile
{
    TileType type;
};

}