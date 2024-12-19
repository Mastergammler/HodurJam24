// need this because of FX mapping
#pragma once

#include "internal.h"
#include "types.h"
#include <unordered_map>

#define NAMEOF(x) #x

static const char DELIMITER = ':';
static const string INPUT_CONF = "configs/keyboard.conf";

Map LoadMap(string name);
Tile TileAt(v2 pos);
v2 PositionOf(TileType type);
