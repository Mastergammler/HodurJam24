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
        // TODO: play whoosh via index or smth
        // -> this is super akward
        if (GameInputs.MoveLeft.pressed)
            PlayAudio(&Audio.UiLeft, {&GlobalStereo});
        if (GameInputs.MoveRight.pressed)
            PlayAudio(&Audio.UiRight, {&GlobalStereo});
        if (GameInputs.MoveForward.pressed)
            PlayAudio(&Audio.UiUp, {&GlobalStereo});
        if (GameInputs.MoveBackward.pressed)
            PlayAudio(&Audio.UiDown, {&GlobalStereo});

        // only play the most recent one
        AudioQueue_ClearSchedule();
        PlayNumberSound(Ui.current_level, BEFORE_NUM);
    }
}
