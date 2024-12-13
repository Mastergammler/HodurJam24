#include "internal.h"

static int currentStepIndex = 0;

void Game_Update()
{
    if (GameInputs.Exit.released) ExitFunction();

    // now, get the steps based on the map tiles!
    // - track player position
    if (GameInputs.MoveForward.pressed)
    {
        currentStepIndex = rand() % Audio.fx_count;
        PlayNewAudio(&Audio.Fx[currentStepIndex]);
    }
    else if (GameInputs.MoveForward.released)
    {
        PlayNewAudio(&Audio.Fx[currentStepIndex]);
    }
}
