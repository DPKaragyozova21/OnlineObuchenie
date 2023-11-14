#pragma once

#include <stdint.h>

namespace fl
{

enum class TileType : uint8_t
{
    NONE,
    MAGNETITE,
    HEMATITE
};

struct Tile
{
    TileType type;
};

}