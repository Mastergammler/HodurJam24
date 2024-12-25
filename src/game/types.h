#pragma once

#include "module.h"

struct HodurInputState : InputState
{
    HodurInputState(int inputCount, KeyInput* inputs)
        : InputState{inputs, inputCount}, Exit(key_states[0]),
          Action(key_states[1]), MoveForward(key_states[2]),
          MoveBackward(key_states[3]), MoveLeft(key_states[4]),
          MoveRight(key_states[5]), TurnLeft(key_states[6]),
          TurnRight(key_states[7]), Log(key_states[8]), Debug(key_states[9]),
          Mixer(key_states[10]), Fire(key_states[11]), Help(key_states[12]),
          Reset(key_states[13]), F1(key_states[14]), F2(key_states[15]),
          F3(key_states[16]), F4(key_states[17])
    {
    }

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
    KeyInput& Help;
    KeyInput& Reset;
    KeyInput& F1;
    KeyInput& F2;
    KeyInput& F3;
    KeyInput& F4;
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
     * No 3
     */
    GRASS,

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
    PILLAR,

    /**
     * Workaround: Just for fx loading ...
     */
    NOOP,

    /**
     * Workaround 2 for next thingy
     */
    HUH,

    /**
     * Workaround 3
     */
    FOLEY
};

struct FxInfo
{
    TileType type;
    int start_idx;
    int count;
};

struct LoadedAudio
{
    // Level Status
    AudioData DangerSound;
    AudioData SuccessSound;
    AudioData FailSound;
    AudioData GameSuccessSound;

    // Gamplay Info
    AudioData OpenChest;
    AudioData LockIn;
    AudioData ObtainKeys;
    AudioData UnlockDoor;

    // Bear
    AudioData BearGrowl;
    AudioData BearBreathingLoop;
    AudioData ProximityYellow;
    AudioData ProximityRed;

    // UI
    AudioData UiDown;
    AudioData UiUp;
    AudioData UiRight;
    AudioData UiLeft;
    AudioData UiEnter;
    AudioData MenuAtmo;

    // Voice
    AudioData HelpInfo;
    AudioData HelpMenu;
    AudioData HelpGame;
    AudioData Intro;
    AudioData ThereIsABear;
    AudioData Victory;

    AudioData* fx;
    int fx_count;
    unordered_map<TileType, FxInfo> fx_mapping;
    unordered_map<TileType, FxInfo> bear_fx_mapping;

    AudioData* ui;
    int ui_count;
    unordered_map<int, int> num_mapping;
};
