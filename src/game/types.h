#pragma once

#include "module.h"

struct v2
{
    int x, y;
};

inline const v2 operator+(const v2 vector, v2 other)
{
    return {vector.x + other.x, vector.y + other.y};
}

inline bool operator==(const v2 vector, v2 other)
{
    return vector.x == other.x && vector.y == other.y;
}

struct HodurInputState : InputState
{
    HodurInputState(int inputCount, KeyInput* inputs)
        : InputState{inputs, inputCount}, Exit(key_states[0]),
          Action(key_states[1]), MoveForward(key_states[2]),
          MoveBackward(key_states[3]), MoveLeft(key_states[4]),
          MoveRight(key_states[5]), TurnLeft(key_states[6]),
          TurnRight(key_states[7]), Log(key_states[8]), Debug(key_states[9]),
          Mixer(key_states[10]), Fire(key_states[11]), Manual(key_states[12]),
          Reset(key_states[13])
    {
    }

    // TODO: FKeys for loading specific levels

    KeyInput& Exit;
    KeyInput& Action;
    KeyInput& MoveForward;
    KeyInput& MoveBackward;
    KeyInput& MoveLeft;
    KeyInput& MoveRight;
    KeyInput& TurnLeft;
    KeyInput& TurnRight;
    KeyInput& Log;
    KeyInput& Debug;
    KeyInput& Mixer;
    KeyInput& Fire;
    KeyInput& Manual;
    KeyInput& Reset;
};

// TODO: this is not a good solution ...
//  i need to do this some way else
enum TileType
{
    /**
     * '-'
     */
    WALL,

    /**
     * No 0
     */
    STONE,

    /**
     * No 1
     */
    CARPET,

    /**
     * No 2
     */
    WOOD,

    /**
     * 'C'
     */
    CHEST,

    /**
     * 'D'
     */
    DOOR,

    /**
     * 'O'
     */
    PILLAR
};

struct FxInfo
{
    TileType type;
    int start_idx;
    int count;
};

struct LoadedAudio
{
    AudioData TestAudio;

    AudioData* Fx;
    int fx_count;

    unordered_map<TileType, FxInfo> fx_mapping;
};

struct Player
{
    v2 position;
};
