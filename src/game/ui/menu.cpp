#include "../internal.h"
#include "../systems.h"
#include "../ui.h"

void Ui_HandleInputs()
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

    // 0 is not a valid level
    if (levelTarget <= 0 || levelTarget > Ui.level_count)
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

void OnUiEnter()
{
    Ui.is_active = true;

    AudioQueue_ClearSchedule();
    Schedule_Clear();

    PlayAudio(&Audio.UiEnter, {&GlobalStereo});
    SchedulePlayback(&Audio.MenuAtmo, {&Ambience, true, true, 0.8}, 0.3);
    PlayNumberSound(Ui.current_level, 0.85);
}

void OnUiExit()
{
    Ui.is_active = false;

    // this needs to happen before the level loads!
    AudioQueue_ClearSchedule();
    StopAudio(Ambience);
    StopAudio(GlobalMono); // number sounds
    StopAudio(GlobalStereo);
}
