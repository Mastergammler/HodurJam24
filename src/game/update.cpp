#include "internal.h"
#include "module.h"

void Game_Update()
{
    if (GameInputs.Exit.released) ExitFunction();
    if (GameInputs.Action.pressed) Log("ACTION!");
}
