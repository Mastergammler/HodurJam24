#include "internal.h"
#include "loading.h"
#include "player.h"
#include "ui.h"

void Game_Update()
{
    Audio_Update(Timer.sim_time);

    if (Ui.is_active)
    {
        if (GameInputs.Action.released) LoadLevel(Ui.current_level);
        if (GameInputs.Exit.released) ExitFunction();

        HandleInputs();
    }
    else
    {
        if (GameInputs.F1.released) LoadLevel(1);
        if (GameInputs.F2.released) LoadLevel(2);
        if (GameInputs.F3.released) LoadLevel(3);
        if (GameInputs.Reset.released) LoadLevel(Level.number);
        if (GameInputs.Exit.released)
        {
            UiEnter();
            return;
        }

        // is this the right order? After the other inputs are handeled?
        CountLockTime();
        HandleMovement();
        HandleActions();
    }
}
