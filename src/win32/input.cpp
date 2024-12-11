#include "module.h"

void Input_ResetKeyStates()
{
    for (int i = 0; i < InputStates->count; i++)
    {
        KeyInput* currentKey = &InputStates->key_states[i];

        currentKey->released = false;
        currentKey->pressed = false;
    }
}

void SetKeyState(bool wasDown, bool isDown, KeyInput& key)
{
    assert(wasDown != isDown);

    if (!wasDown && isDown)
    {
        key.pressed = true;
        key.is_down = true;
    }
    else
    {
        key.released = true;
        key.is_down = false;
    }
}

/**
 * Handles basic key presses, but no key combinations (Ctrl + Key | Alt +
 * Key)
 * Supports remapping through the file
 */
void HandleKeyboardInput(LPARAM lParam, WPARAM wParam)
{
    /*
     * lParam is a bit set, and on the 30th position it tells us if the
     * key was down before
     * lparam & (1 << 30) gives us either 0 OR the 30th bit beeing set
     * (which is way bigger than 1 => 2^30)
     */
    bool wasDown = ((lParam & (1 << 30)) != 0);
    bool isDown = ((lParam & (1 << 31)) == 0);
    uint32_t vkCode = wParam;

    if (isDown == wasDown)
    {
        return;
    }

    for (int i = 0; i < InputStates->count; i++)
    {
        KeyInput* input = &InputStates->key_states[i];
        if (input->keycode == vkCode)
        {
            SetKeyState(wasDown, isDown, *input);
        }
    }

    // bool altKeyWasDown = (lParam & (1 << 29)) != 0;
}
