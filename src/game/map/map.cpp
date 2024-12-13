#include "../internal.h"

static Tile NULL_TILE = {WALL, v2{-1, -1}, false};

Tile TileAt(v2 pos)
{
    if (pos.x < 0 || pos.x >= CurrentMap.columns) return NULL_TILE;
    if (pos.y < 0 || pos.y >= CurrentMap.rows) return NULL_TILE;

    int positionIndex = CurrentMap.columns * pos.y + pos.x;
    return CurrentMap.tiles[positionIndex];
}

v2 PositionOf(TileType type)
{
    Tile* tilePointer = CurrentMap.tiles;

    for (int row = 0; row < CurrentMap.rows; row++)
    {
        for (int col = 0; col < CurrentMap.columns; col++)
        {
            Tile cur = *tilePointer++;
            if (cur.type == type) return {col, row};
        }
    }
    // TODO: not quite sure about this, if this makes sense
    return {-1, -1};
}
