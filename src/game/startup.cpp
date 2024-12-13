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
    GameInputs.Reset.identifier = TrimToVariableName(NAMEOF(GameInputs.Reset));

    unordered_map<string, string> keyMappings = read_kvp_file(INPUT_CONF,
                                                              DELIMITER);
    Input_Init(&GameInputs, keyMappings);
}

void LoadAudio()
{
    Clock loadAudioClock = {};
    Measure_Start(loadAudioClock);

    vector<FileEntry> files = directory_files("res/fx");

    Audio.fx_count = files.size();
    Audio.Fx = new AudioData[Audio.fx_count]();

    FxInfo lastInfo = {};
    for (int i = 0; i < Audio.fx_count; i++)
    {
        for (const auto& pair : FX_MAPPING)
        {
            if (starts_with(files[i].file_name, pair.first))
            {
                if (lastInfo.type != pair.second)
                {
                    if (i > 0)
                    {
                        Audio.fx_mapping.insert({lastInfo.type, lastInfo});
                        lastInfo = {};
                    }
                    lastInfo.start_idx = i;
                    lastInfo.type = pair.second;
                }

                lastInfo.count++;
                break;
            }
        }

        // TODO: very ugly logic
        if (i == Audio.fx_count - 1)
        {
            // insert the last one
            Audio.fx_mapping.insert({lastInfo.type, lastInfo});
        }

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

    Log("Game initalized successfully");
}

void Game_Dispose()
{
    Audio_Dispose();
    Log("Game: Happy exit.");
}
