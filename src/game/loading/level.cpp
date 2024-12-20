#include "../internal.h"
#include "../map.h"
#include "../ui.h"

// TODO: Do i maybe want to specify the start position in the map?
//  - This would give more flexibility
//  - But also make the game yet a bit harder?
//  -> The Door -> Chest -> Door makes quite sense i think
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

    if (Level.map.bear_present)
    {
        Bear.position = Level.map.bear_start;
        Bear.is_present = true;
        Logf("Bear start position: (%i,%i)", Bear.position.x, Bear.position.y);
    }
    else
    {
        Bear.is_present = false;
    }

    PlayNumberSound(level, 1.6);
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
