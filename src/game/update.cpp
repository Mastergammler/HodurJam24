#include "internal.h"

void Game_Update()
{
    if (GameInputs.Exit.released) ExitFunction();
    if (GameInputs.Action.pressed) PlayTest("res/TrashSong_Mix2_mq.ogg");
}
