#include "../internal.h"
#include "../map.h"
#include "../ui.h"

v2 DeterminePlayerStart()
{
    v2 doorPos = PositionOf(DOOR);

    v2 playerStart = doorPos + NORTH;
    if (TileAt(playerStart).is_walkable) return playerStart;
    playerStart = doorPos + EAST;
    if (TileAt(playerStart).is_walkable) return playerStart;
    playerStart = doorPos + SOUTH;
    if (TileAt(playerStart).is_walkable) return playerStart;
    playerStart = doorPos + WEST;
    if (TileAt(playerStart).is_walkable) return playerStart;

    Logf("No valid start position found for: %s", Level.level_name.c_str());

    // either (-1,-1) or door is completely surrounded by walls
    return doorPos + INIT;
}

void LoadLevel(int level)
{
    OnUiExit();
    // TODO: stop all current audio?!

    string levelName = format("test0%i", level);

    Level.level_name = levelName;
    Level.number = level;
    Level.player_has_key = false;
    Level.map = LoadMap(levelName);

    delete Nodes.items;
    Nodes.max_size = Level.map.total_tiles;
    Nodes.items = new NodeItem[Nodes.max_size]();
    Nodes.count = 0;

    Player.position = DeterminePlayerStart();
    Player.in_walk_anim = false;
    Player.time_since_anim_start = 0;
    Player.inputs_locked = false;

    // TODO: bear loading -> via map etc
    Bear.position = {0, 0};

    PlayNumberSound(level, 1.8);
    PlayAudio(&Audio.DangerSound, {&GlobalStereo});
    PlayAudio(&Audio.LockIn, {&GlobalStereo, false});
    Logf("Loading level %s (%i tiles)",
         levelName.c_str(),
         Level.map.total_tiles);
}

void LoadNextLevel()
{
    if (Ui.current_level >= Ui.level_count)
    {
        Logf("No next level available, last level is %i", Ui.current_level);
        return;
    }

    LoadLevel(++Ui.current_level);
}
