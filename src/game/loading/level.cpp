#include "../audio.h"
#include "../events.h"
#include "../internal.h"
#include "../map.h"
#include "../systems.h"
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

    int levelIdx = level - 1;

    Level.level_name = Ui.level_names[levelIdx];
    Level.number = level;
    Level.player_has_key = false;
    Level.map = LoadMap(Level.level_name);
    Logf("Loading level %s (%i tiles)",
         Level.level_name.c_str(),
         Level.map.total_tiles);

    delete Nodes.items;
    Nodes.max_size = Level.map.total_tiles;
    Nodes.items = new NodeItem[Nodes.max_size]();
    Nodes.count = 0;

    Player.position = DeterminePlayerStart();
    Player.in_walk_anim = false;
    Player.time_since_anim_start = 0;

    if (Level.map.bear_present)
    {
        Bear.position = Level.map.bear_start;
        Bear.is_present = true;
        Logf("Bear start position: (%i,%i)", Bear.position.x, Bear.position.y);

        PlayAudio(&Audio.ProximityYellow, {&ProximityYellow, true, true, 0});
        PlayAudio(&Audio.ProximityRed, {&ProximityRed, true, true, 0});
        PlayAudio(&Audio.BearBreathingLoop, {&Bear.breathing, true, true, 0});
    }
    else
    {
        Bear.is_present = false;
    }

    Event_LevelStart();
}

void LoadNextLevel()
{
    if (Ui.current_level >= Ui.level_count)
    {
        Logf("No next level available, last level is %i, player beat the game.",
             Ui.current_level);
        Event_GameVictory();
        return;
    }

    LoadLevel(++Ui.current_level);
}
