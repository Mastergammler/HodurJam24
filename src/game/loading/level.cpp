#include "../internal.h"
#include "../map.h"
#include "../ui.h"

void LoadLevel(int level)
{
    Ui.is_active = false;

    // TODO: play reset sound
    // stop all current audio
    string levelName = format("test0%i", level);

    Level.level_name = levelName;
    Level.number = level;
    Level.has_key = false;
    Level.map = LoadMap(levelName);
    v2 doorPos = PositionOf(DOOR);

    // TODO: not flexible, if the door is on the sides
    v2 playerStart = doorPos + v2{0, -1};
    Player.position = playerStart;
    Player.in_animation = false;
    Player.time_since_anim_start = 0;

    // TODO: i would like to play it with delay . (delay .. delay ...)
    PlayNumberSound(level);
    PlayAudio(&Audio.DangerSound, {&GlobalStereo}, true);
    PlayAudio(&Audio.LockIn, {&GlobalStereo}, false);
    Logf("Loading level %s", levelName.c_str());
}
