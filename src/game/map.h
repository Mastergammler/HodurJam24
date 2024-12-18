// need this because of FX mapping
#pragma once

#include "internal.h"
#include "types.h"
#include <unordered_map>

#define NAMEOF(x) #x

static const char DELIMITER = ':';
static const string INPUT_CONF = "configs/keyboard.conf";

static unordered_map<string, TileType> FX_MAPPING = {
                                        {"footstep-stone", STONE},
                                        {"footstep-carpet", CARPET},
                                        {"footstep-wood-hi", WOOD},
                                        {"footsteps-grass", GRASS},
                                        {"wall-bump", WALL},
                                        {"chest-bump", CHEST},
                                        {"door-bump", DOOR},
                                        {"pillar-bump", PILLAR},
                                        // TODO: I keep adding things that are
                                        // not actually tiles
                                        //  -> I have to find another solution
                                        {"nope-sound", NOOP},
                                        {"huh", HUH},
                                        {"pocket-keys", POCKET},
};

static unordered_map<string, TileType> BEAR_FX_MAPPING = {
                                        {"footsteps-bear-stone", STONE},
                                        {"footsteps-bear-stone", CARPET},
                                        {"footsteps-bear-stone", WOOD},
                                        {"footsteps-bear-stone", GRASS}

};

Map LoadMap(string name);
Tile TileAt(v2 pos);
v2 PositionOf(TileType type);
