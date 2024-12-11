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

struct InputState
{
    KeyInput* key_states;
    int count;
};

static InputState* InputStates;

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
void Input_ResetKeyStates(InputState* state);
void HandleMessages(HWND window);

/**
 * Loads the key configuration for input handling
 */
void Input_Init(InputState* inputs, unordered_map<string, string> keyMappings);
