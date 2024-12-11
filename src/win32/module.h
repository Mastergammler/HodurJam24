#pragma once

#include "../logging/module.h"
#include "imports.h"

struct KeyInput
{
    /**
     * Name of the key used, that is user understandable
     * Name that can be used in text, etc
     */
    string key_name = "init";

    /**
     * Key identifier, that corresponds to the mapping file
     */
    string identifier = "init";

    /**
     * Keycode used by the Win32 Event system to identify keys
     */
    uint32_t keycode;

    /**
     * If the button is down right now (this frame)
     */
    bool is_down;

    /**
     * Single frame state, when the button just got pressed (this frame)
     */
    bool pressed;

    /**
     * Single frame state, when the button just got released (this frame)
     */
    bool released;
};

// TODO: this should not be module dependant, i should have this somewhere else
// ... -> this should really live in the game module
struct InputState
{
    static const int KeyCount = 14;
    KeyInput All[KeyCount];

    KeyInput& Exit = All[0];
    KeyInput& Action = All[1];
    KeyInput& MoveForward = All[2];
    KeyInput& MoveBackward = All[3];
    KeyInput& MoveLeft = All[4];
    KeyInput& MoveRight = All[5];
    KeyInput& TurnLeft = All[6];
    KeyInput& TurnRight = All[7];
    KeyInput& Log = All[8];
    KeyInput& Debug = All[9];
    KeyInput& Mixer = All[10];
    KeyInput& Fire = All[11];
    KeyInput& Manual = All[12];
    KeyInput& Skip = All[13];
};

static InputState Input;

HWND InitializeWindow(string name,
                      HINSTANCE instance,
                      ExitCallback closeProgram);

/**
 * Reads the cmd arguments the program was started with
 */
vector<string> ReadArguments();

/**
 * Resets the pressed and released states for each key,
 * because these are one time events
 *
 * Needs to be called every time before messages are handled
 */
void Input_ResetKeyStates();
void HandleMessages(HWND window);

/**
 * Loads the key configuration for input handling
 */
void Input_Init(unordered_map<string, string> keyMappings);
