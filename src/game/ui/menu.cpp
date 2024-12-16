#include "../internal.h"
#include "../ui.h"

void HandleInputs()
{
    int offset = 0;

    if (GameInputs.MoveLeft.pressed)
        offset = -1;
    else if (GameInputs.MoveRight.pressed)
        offset = 1;
    else if (GameInputs.MoveForward.pressed)
        offset = -5;
    else if (GameInputs.MoveBackward.pressed)
        offset = 5;
    else
        return;

    int levelTarget = Ui.current_level + offset;

    if (levelTarget < 0 || levelTarget > Ui.level_count)
    {
        if (Audio.fx_mapping.find(NOOP) != Audio.fx_mapping.end())
        {
            FxInfo fxInfo = Audio.fx_mapping[NOOP];
            int idxOffset = rand() % fxInfo.count;
            AudioData* audio = &Audio.fx[fxInfo.start_idx + idxOffset];
            PlayAudio(audio, {&GlobalMono});
        }
    }
    else
    {
        Ui.current_level += offset;
        PlayNumberSound(Ui.current_level);
    }
}
