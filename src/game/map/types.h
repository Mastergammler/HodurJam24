#include "../imports.h"
#include "../types.h"

struct Tile
{
    TileType type;
    v2 position;
    bool is_walkable;
    bool is_interactable;
};

inline ostream& operator<<(ostream& os, const Tile& obj)
{
    os << "( " << obj.position.x << " | " << obj.position.y << " ) "
       << obj.type;
    return os;
}

/*
 * Definition start Upper left
 */
struct Map
{
    Map(int columns, int rows)
    {
        this->columns = columns;
        this->rows = rows;
        this->total_tiles = columns * rows;

        tiles = new Tile[this->total_tiles];
    }

    Tile* tiles;
    int rows;
    int columns;
    int total_tiles;
};
