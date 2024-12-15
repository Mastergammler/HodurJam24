// need this because of FX mapping
#pragma once

#include "internal.h"
#include "types.h"

#define NAMEOF(x) #x

static const char DELIMITER = ':';
static const string INPUT_CONF = "configs/keyboard.conf";

static unordered_map<string, TileType> FX_MAPPING = {
                                        {"footstep-stone", STONE},
                                        {"footstep-carpet", CARPET},
                                        {"footstep-wood-hi", WOOD},
                                        {"wall-bump", WALL},
                                        {"chest-bump", CHEST},
                                        {"door-bump", DOOR},
                                        {"pillar-bump", PILLAR},
                                        {"nope-sound", NOOP}};

Map LoadMap(string name);
Tile TileAt(v2 pos);
v2 PositionOf(TileType type);
