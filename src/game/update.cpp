#include "internal.h"
#include "map.h"
#include "player.h"

void ResetLevel()
{
    // TODO: play reset sound

    string currentLevel = "test01";
    CurrentMap = LoadMap(currentLevel);
    v2 doorPos = PositionOf(DOOR);

    // TODO: not flexible, if the door is on the sides
    v2 playerStart = doorPos + v2{0, -1};
    Player = {playerStart};

    Logf("Reset level %s", currentLevel.c_str());
}

void Game_Update()
{
    if (GameInputs.Exit.released) ExitFunction();
    if (GameInputs.Reset.released) ResetLevel();

    HandleMovement();
}
