#include "../internal.h"
#include "../map.h"
#include "../ui.h"

void LoadLevel(int level)
{
    UiExit();

    // TODO: play reset sound
    // stop all current audio
    string levelName = format("test0%i", level);

    Level.level_name = levelName;
    Level.number = level;
    Level.player_has_key = false;
    Level.map = LoadMap(levelName);
    v2 doorPos = PositionOf(DOOR);

    delete Nodes.items;
    Nodes.max_size = Level.map.total_tiles;
    Nodes.items = new NodeItem[Nodes.max_size]();
    Nodes.count = 0;
    Logf("Max node size is: %i", Nodes.max_size);

    v2 playerStart = doorPos + v2{0, -1};
    Player.position = playerStart;
    Player.in_walk_anim = false;
    Player.time_since_anim_start = 0;
    Player.inputs_locked = false;

    // TODO: bear loading -> via map etc
    Bear.position = {0, 0};

    PlayNumberSound(level, 2);
    PlayAudio(&Audio.DangerSound, {&GlobalStereo});
    PlayAudio(&Audio.LockIn, {&GlobalStereo, false});
    Logf("Loading level %s", levelName.c_str());
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
