#include "../internal.h"

static Tile NULL_TILE = {WALL, v2{-1, -1}, false};

Tile TileAt(v2 pos)
{
    if (pos.x < 0 || pos.x >= Level.map.columns) return NULL_TILE;
    if (pos.y < 0 || pos.y >= Level.map.rows) return NULL_TILE;

    int positionIndex = Level.map.columns * pos.y + pos.x;
    return Level.map.tiles[positionIndex];
}

v2 PositionOf(TileType type)
{
    Tile* tilePointer = Level.map.tiles;

    for (int row = 0; row < Level.map.rows; row++)
    {
        for (int col = 0; col < Level.map.columns; col++)
        {
            Tile cur = *tilePointer++;
            if (cur.type == type) return {col, row};
        }
    }
    // TODO: not quite sure about this, if this makes sense
    return {-1, -1};
}
