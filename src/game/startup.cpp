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

void Game_Init(ExitCallback exitFunction)
{
    ExitFunction = exitFunction;
    InitInputs();
    Audio_Init();

    // TODO: test loading time
    Map map = LoadMap("test01");

    for (int i = 0; i < map.total_tiles; i++)
    {
        Tile t = map.tiles[i];
        // Logf("%i: (%i,%i)", t.type, t.position.x, t.position.y);
    }

    Log("Game initalized successfully");
}

void Game_Dispose()
{
    Audio_Dispose();
    Log("Game: Happy exit.");
}
