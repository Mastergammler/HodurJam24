#include "../imports.h"

struct Vector2
{
    int x, y;
};

enum TileType
{
    /**
     * '-'
     */
    WALL,

    /**
     * No 0
     */
    STONE,

    /**
     * No 1
     */
    MOSSY_STONE,

    /**
     * No 2
     */
    WOOD,

    /**
     * 'C'
     */
    CHEST,

    /**
     * 'D'
     */
    DOOR,

    /**
     * 'O'
     */
    PILLAR
};

struct Tile
{
    TileType type;
    Vector2 position;
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
