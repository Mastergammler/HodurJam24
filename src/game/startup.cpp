#include "events.h"
#include "internal.h"
#include "loading.h"
#include "map.h"
#include "systems.h"
#include "utils/parsing.cpp"

void InitInputs()
{
    // TODO: renames within the macro don't work -> bit annoying
    GameInputs.Exit.identifier = TrimToVariableName(NAMEOF(GameInputs.Exit));
    GameInputs.Action.identifier = TrimToVariableName(
                                            NAMEOF(GameInputs.Action));
    GameInputs.TurnLeft.identifier = TrimToVariableName(
                                            NAMEOF(GameInputs.TurnLeft));
    GameInputs.TurnRight.identifier = TrimToVariableName(
                                            NAMEOF(GameInputs.TurnRight));
    GameInputs.MoveForward.identifier = TrimToVariableName(
                                            NAMEOF(GameInputs.MoveForward));
    GameInputs.MoveBackward.identifier = TrimToVariableName(
                                            NAMEOF(GameInputs.MoveBackward));
    GameInputs.MoveLeft.identifier = TrimToVariableName(
                                            NAMEOF(GameInputs.MoveLeft));
    GameInputs.MoveRight.identifier = TrimToVariableName(
                                            NAMEOF(GameInputs.MoveRight));
    GameInputs.Log.identifier = TrimToVariableName(NAMEOF(GameInputs.Log));
    GameInputs.Debug.identifier = TrimToVariableName(NAMEOF(GameInputs.Debug));
    GameInputs.Mixer.identifier = TrimToVariableName(NAMEOF(GameInputs.Mixer));
    GameInputs.Fire.identifier = TrimToVariableName(NAMEOF(GameInputs.Fire));
    GameInputs.Help.identifier = TrimToVariableName(NAMEOF(GameInputs.Help));
    GameInputs.Reset.identifier = TrimToVariableName(NAMEOF(GameInputs.Reset));
    GameInputs.F1.identifier = TrimToVariableName(NAMEOF(GameInputs.F1));
    GameInputs.F2.identifier = TrimToVariableName(NAMEOF(GameInputs.F2));
    GameInputs.F3.identifier = TrimToVariableName(NAMEOF(GameInputs.F3));
    GameInputs.F4.identifier = TrimToVariableName(NAMEOF(GameInputs.F4));

    unordered_map<string, string> keyMappings = read_kvp_file(INPUT_CONF,
                                                              DELIMITER);
    Input_Init(&GameInputs, keyMappings);
}

void CreateVoices()
{
    // TODO: better choosing of the fx
    CreateVoiceForAudio(&Audio.fx[0], Player.left_foot);
    CreateVoiceForAudio(&Audio.fx[0], Player.right_foot);
    CreateVoiceForAudio(&Audio.fx[0], Player.body);
    CreateVoiceForAudio(&Audio.fx[0], Bear.body);
    CreateVoiceForAudio(&Audio.fx[0], Bear.breathing);
    CreateVoiceForAudio(&Audio.ui[0], GlobalMono);
    CreateVoiceForAudio(&Audio.DangerSound, GlobalStereo);
    CreateVoiceForAudio(&Audio.MenuAtmo, Ambience);
    CreateVoiceForAudio(&Audio.ProximityYellow, ProximityYellow);
    CreateVoiceForAudio(&Audio.ProximityRed, ProximityRed);
    CreateVoiceForAudio(&Audio.Victory, Voiceline);
}

// PERF: load most of it in the bg else loading might be quite long
void LoadAudio()
{
    Clock loadAudioClock = {};
    Measure_Start(loadAudioClock);

    LoadFx();
    LoadUiSounds();
    LoadStaticAudio();

    float time = Measure_Elapsed(loadAudioClock);
    Logf("||: All audio loaded within %.2fms :||", time);

    CreateVoices();
}

void LoadMaps()
{
    vector<FileEntry> files = directory_files("res/maps");

    Ui.level_names = new string[files.size()];
    Ui.level_count = files.size();
    Ui.current_level = 1;

    for (int i = 0; i < files.size(); i++)
    {
        Ui.level_names[i] = files[i].file_name;
    }

    Logf("Detected %i level maps.", Ui.level_count);
}

void Game_Init(ExitCallback exitFunction)
{
    ExitFunction = exitFunction;

    InitInputs();
    Audio_Init();
    LoadAudio();
    LoadMaps();

    Schedule_Init(8);
    Event_GameStart();

    Log("Game initalized successfully");
}

void Game_Dispose()
{
    Audio_Dispose();
    Log("Game: Happy exit.");
}
