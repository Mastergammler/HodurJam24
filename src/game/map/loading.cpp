#include "../internal.h"

const char* MAP_REL_DIR = "res/maps/";
const char* MAP_EXT = ".map";
const char COL_SEP = ' ';

TileType ParseTileType(char c)
{
    switch (toupper(c))
    {
        case '0': return STONE;
        case '1': return CARPET;
        case '2': return WOOD;
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
        case WOOD: return true;
        default: return false;
    }
}

// TODO: I can impove this to be way more flexible
//  - i don't need to do the akward column check
//  - i have it in a way, where every element is only 1 char,
//    thefore i can parse it accordingly
//  -> Even with only whitespace, so it still works!!!
//  - I can even go through it and just parse by char count, without a need for
//  a separator
Map LoadMap(string name)
{
    Clock fileClock = {};
    Measure_Start(fileClock);

    bool validMap = true;
    vector<string> lines = read_all_lines(MAP_REL_DIR + name + MAP_EXT);
    vector<char> columnData;

    int colCount = 0;

    for (int row = 0; row < lines.size(); row++)
    {
        int currentCol = 0;
        string line = lines[row];

        for (int c = 0; c < line.length(); c++)
        {
            char cur = line[c];
            if (cur == COL_SEP) continue;
            columnData.push_back(cur);
            currentCol++;
        }

        if (row == 0)
            colCount = currentCol;
        else if (colCount != currentCol)
        {
            Logf("Invalid data for file %s, row %i, expected %i columns but "
                 "found %i",
                 name.c_str(),
                 row,
                 colCount,
                 currentCol);
            validMap = false;
        }
    }

    if (validMap)
    {
        Map map = {colCount, (int)lines.size()};

        for (int i = 0; i < map.total_tiles; i++)
        {
            Tile* currentTile = &map.tiles[i];
            // truncation is desired!
            int curRow = i / colCount;
            int curCol = i % colCount;

            currentTile->type = ParseTileType(columnData[i]);
            currentTile->position = {curCol, curRow};
            currentTile->is_walkable = IsWalkable(currentTile->type);
        }

        float time = Measure_Elapsed(fileClock);
        Logf("Successfully loaded map %s within %.2fms", name.c_str(), time);

        return map;
    }

    return Map{0, 0};
}
