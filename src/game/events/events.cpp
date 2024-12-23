#include "../audio.h"
#include "../bear.h"
#include "../internal.h"
#include "../loading.h"
#include "../map.h"
#include "../systems.h"
#include "../ui.h"

// TODO: fix this, just detect the first bear level
const int FIRST_BEAR_LEVEL = 10;

void LoadFirstLevel()
{
    LoadLevel(1);
}

void Event_GameStart()
{
    Player.inputs_locked = true;
    PlayAudio(&Audio.HelpInfo, {&Voiceline});
    // For some reason it all starts too early?
    ScheduleExecution(Audio.HelpInfo.length_in_s + 1.5, LoadFirstLevel);
}

void UnlockPlayer()
{
    Player.inputs_locked = false;
}

void SetBearAudioLevels()
{
    Bear_MoveTowardsPlayer(false);
}

void Event_ClearGameAudio()
{
    StopAudio(ProximityYellow);
    StopAudio(ProximityRed);
    StopAudio(Bear.breathing);

    AudioQueue_ClearSchedule();
    Schedule_Clear();
}

void Event_LevelStart()
{
    v2 doorPos = PositionOf(DOOR);
    v2 direction = doorPos - Player.position;

    float audioDelay = 0;

    PlaybackSettings playback = DirectionalAudio(direction);
    playback.voice = &GlobalStereo;
    playback.interrupt_previous = false;
    PlayAudio(&Audio.LockIn, playback);
    audioDelay = Audio.LockIn.length_in_s - 1;

    if (!Level.intro_played && !Level.map.bear_present)
    {
        PlaybackSettings playback = DirectionalAudio(direction);
        playback.voice = &Voiceline;
        SchedulePlayback(&Audio.Intro, playback, audioDelay);
        audioDelay += Audio.Intro.length_in_s;
        Level.intro_played = true;
    }
    else if (!Level.bear_info_played && Level.map.bear_present)
    {
        PlaybackSettings playback = DirectionalAudio(direction);
        playback.voice = &Voiceline;
        SchedulePlayback(&Audio.Intro, playback, audioDelay);
        audioDelay += Audio.Intro.length_in_s;
        SchedulePlayback(&Audio.ThereIsABear, playback, audioDelay);
        audioDelay += Audio.ThereIsABear.length_in_s;
        Level.bear_info_played = true;
    }

    ScheduleExecution(audioDelay, UnlockPlayer);
    ScheduleExecution(audioDelay, SetBearAudioLevels);
    SchedulePlayback(&Audio.DangerSound, {&GlobalStereo}, audioDelay);
    PlayNumberSound(Ui.current_level, audioDelay + .65);
}

void Event_LevelVictory()
{
    Event_ClearGameAudio();

    Player.inputs_locked = true;
    PlayAudio(&Audio.UnlockDoor, {&GlobalStereo});
    PlayAudio(&Audio.SuccessSound, {&GlobalStereo, false});
    float waitTime = Audio.UnlockDoor.length_in_s +
                     Audio.SuccessSound.length_in_s;
    ScheduleExecution(waitTime, LoadNextLevel);
}

void Event_ResetLevel()
{
    Event_ClearGameAudio();
    LoadLevel(Ui.current_level);
}

void Event_LevelFailure()
{
    Event_ClearGameAudio();

    Player.inputs_locked = true;
    PlaybackSettings playback = {&GlobalStereo};
    playback.volume = 1.5f;
    PlayAudio(&Audio.FailSound, playback);
    PlaybackSettings p2 = {&Ambience};
    p2.volume = 2.f;
    SchedulePlayback(&Audio.BearGrowl, p2, 3.25);
}

void Event_GameVictory()
{
    PlayAudio(&Audio.GameSuccessSound, {&GlobalStereo});
    SchedulePlayback(&Audio.Victory,
                     {&Voiceline},
                     Audio.GameSuccessSound.length_in_s);
}
