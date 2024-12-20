#include "../internal.h"

const char* MAP_REL_DIR = "res/maps/";
const char* MAP_EXT = ".map";
const char COL_SEP = ' ';
const char DATA_INDICATOR = ':';
/**
 *  Wall can be anything, becaues it's the default, but this is for readability
 */
const char WALL_SYMBOL = '-';

TileType ParseTileType(char c)
{
    switch (toupper(c))
    {
        case '0': return STONE;
        case '1': return CARPET;
        case '2': return WOOD;
        case '3': return GRASS;
        case 'C': return CHEST;
        case 'D': return DOOR;
        case 'O': return PILLAR;
        default: return WALL;
    }
}

// TODO: this should maybe be a definition file or something?
bool IsWalkable(TileType type)
{
    switch (type)
    {
        case STONE:
        case CARPET:
        case WOOD:
        case GRASS: return true;
        default: return false;
    }
}

bool IsInteractable(TileType type)
{
    switch (type)
    {
        case DOOR:
        case CHEST: return true;
        default: return false;
    }
}

Map ParseMap(vector<string> lines)
{
    int maxCol = 0;
    for (int i = 0; i < lines.size(); i++)
    {
        if (lines[i].length() > 0 && lines[i][0] == DATA_INDICATOR) continue;

        // every 2 chars we have 1 tile element, and one empty one
        // for readability purposes
        // truncation of uneven numbers desired!
        int dataLength = (lines[i].length() + 1) / 2;
        if (dataLength > maxCol) maxCol = dataLength;
    }

    vector<char> columnData;
    int rowCount = 0;

    for (int lIdx = 0; lIdx < lines.size(); lIdx++)
    {
        string line = lines[lIdx];
        int lineCols = (line.length() + 1) / 2;
        int missingCols = maxCol - lineCols;

        if (line.length() == 0) continue;

        // TODO: parse data
        if (line[0] == DATA_INDICATOR) continue;

        // columns are separated by one whitespace always!
        for (int c = 0; c < line.length(); c += 2)
        {
            columnData.push_back(line[c]);
        }

        // even out the data, for easier processing
        for (int fill = 0; fill < missingCols; fill++)
        {
            columnData.push_back(WALL_SYMBOL);
        }

        rowCount++;
    }

    Map map = {maxCol, rowCount};

    for (int i = 0; i < map.total_tiles; i++)
    {
        Tile* currentTile = &map.tiles[i];
        // truncation is desired!
        int curRow = i / maxCol;
        int curCol = i % maxCol;

        currentTile->type = ParseTileType(columnData[i]);
        currentTile->position = {curCol, curRow};
        currentTile->is_walkable = IsWalkable(currentTile->type);
        currentTile->is_interactable = IsInteractable(currentTile->type);
    }

    return map;
}

Map LoadMap(string name)
{
    Clock fileClock = {};
    Measure_Start(fileClock);

    bool validMap = true;
    vector<string> lines = read_all_lines(MAP_REL_DIR + name + MAP_EXT);
    Map map = ParseMap(lines);

    if (lines.size() > 0)
    {
        float time = Measure_Elapsed(fileClock);
        Logf("Successfully loaded map %s within %.2fms", name.c_str(), time);
    }

    return map;
}
