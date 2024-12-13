#include "internal.h"
#include "map.h"
#include "player.h"

void ResetLevel(string levelName)
{
    // TODO: play reset sound
    CurrentMap = LoadMap(levelName);
    v2 doorPos = PositionOf(DOOR);

    // TODO: not flexible, if the door is on the sides
    v2 playerStart = doorPos + v2{0, -1};
    Player = {playerStart};

    Logf("Reset level %s", levelName.c_str());
}

void Game_Update()
{
    if (GameInputs.Exit.released) ExitFunction();
    if (GameInputs.F1.released) ResetLevel("test01");
    if (GameInputs.F2.released) ResetLevel("test02");
    if (GameInputs.F3.released) ResetLevel("test03");

    HandleMovement();
}
