#include "internal.h"
#include "loading.h"
#include "player.h"
#include "systems.h"
#include "ui.h"

void Game_Update()
{
    Audio_Update(Timer.sim_time);
    Schedule_Update(Timer.sim_time);

    if (Ui.is_active)
    {
        if (GameInputs.Action.released)
        {
            OnUiExit();
            LoadLevel(Ui.current_level);
        }

        if (GameInputs.Exit.released) ExitFunction();

        Ui_HandleInputs();
    }
    else
    {
        if (GameInputs.Reset.released) LoadLevel(Level.number);
        if (GameInputs.Exit.released)
        {
            OnUiEnter();
            return;
        }

        if (Player.inputs_locked) return;

        // TODO: handle inputs
        HandleMovement();
        HandleActions();
    }
}
