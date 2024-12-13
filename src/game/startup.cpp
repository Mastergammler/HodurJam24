#include "internal.h"
#include "map.h"
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
    GameInputs.Manual.identifier = TrimToVariableName(
                                            NAMEOF(GameInputs.Manual));
    GameInputs.Skip.identifier = TrimToVariableName(NAMEOF(GameInputs.Skip));

    unordered_map<string, string> keyMappings = read_kvp_file(INPUT_CONF,
                                                              DELIMITER);
    Input_Init(&GameInputs, keyMappings);
}

void LoadAudio()
{
    Clock loadAudioClock = {};
    Measure_Start(loadAudioClock);

    // LoadOggAsPcm(Audio.TestAudio, "res/TrashSong_Mix2_mq.ogg");

    vector<FileEntry> files = directory_files("res/fx");

    Audio.fx_count = files.size();
    Audio.Fx = new AudioData[Audio.fx_count]();
    for (int i = 0; i < Audio.fx_count; i++)
    {
        LoadOggAsPcm(Audio.Fx[i], files[i].path);
    }

    float time = Measure_Elapsed(loadAudioClock);
    Logf("|::| All audio loaded within %.2fms", time);
}

void Game_Init(ExitCallback exitFunction)
{
    ExitFunction = exitFunction;
    InitInputs();
    Audio_Init();

    // TODO: do in background, else it takes forever to start etc
    LoadAudio();

    Map map = LoadMap("test01");

    Log("Game initalized successfully");
}

void Game_Dispose()
{
    Audio_Dispose();
    Log("Game: Happy exit.");
}
