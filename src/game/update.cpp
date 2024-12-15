#include "internal.h"
#include "map.h"
#include "player.h"

void LoadLevel(string levelName)
{
    // TODO: play reset sound
    // stop all current audio

    Level.level_name = levelName;
    Level.has_key = false;
    Level.map = LoadMap(levelName);
    v2 doorPos = PositionOf(DOOR);

    // TODO: not flexible, if the door is on the sides
    v2 playerStart = doorPos + v2{0, -1};
    Player.position = playerStart;
    Player.in_animation = false;
    Player.time_since_anim_start = 0;

    PlayAudioNow(&Audio.DangerSound, {&GlobalVoice});
    Logf("Reset level %s", levelName.c_str());
}

void Game_Update()
{
    if (GameInputs.Exit.released) ExitFunction();
    if (GameInputs.F1.released) LoadLevel("test01");
    if (GameInputs.F2.released) LoadLevel("test02");
    if (GameInputs.F3.released) LoadLevel("test03");
    if (GameInputs.Reset.released) LoadLevel(Level.level_name);

    HandleMovement();
    HandleActions();
}
