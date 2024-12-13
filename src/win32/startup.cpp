#include "module.h"

KeyInput* FindInput(InputState* input, string name)
{
    for (int i = 0; i < input->count; i++)
    {
        if (input->key_states[i].identifier == name)
            return &input->key_states[i];
    }

    return NULL;
}

bool IsChar(const string& s)
{
    return s.length() == 1;
}

/**
 * NOTE: map is an orderered map and has therefore O(log n) lookup times
 * while the unordered map is a hash map with O(n) lookup time
 */
static const unordered_map<string, size_t> SpecialCharacterMap = {
                                        {"SPACE", VK_SPACE},
                                        {"ESC", VK_ESCAPE},
                                        {"ARROW_UP", VK_UP},
                                        {"ARROW_DOWN", VK_DOWN},
                                        {"ARROW_LEFT", VK_LEFT},
                                        {"ARROW_RIGHT", VK_RIGHT},
                                        {"COMMA", VK_OEM_COMMA},
                                        {"PERIOD", VK_OEM_PERIOD},
                                        {"CTRL", VK_CONTROL},
                                        {"SHIFT", VK_SHIFT},
                                        {"ALT", VK_MENU},
                                        {"TAB", VK_TAB},
                                        {"QM", VK_OEM_2},
                                        {"F1", VK_F1},
                                        {"F2", VK_F2},
                                        {"F3", VK_F3},
                                        {"F4", VK_F4}};

size_t GetSpecialKeyCode(const string& keyId)
{
    if (SpecialCharacterMap.count(keyId) > 0)
    {
        return SpecialCharacterMap.find(keyId)->second;
    }
    else
    {
        Logf("No special key with the name '%s' defined!", keyId.c_str());
        return VK_NONAME;
    }
}

// TODO: Feature: direct keycode input -> workaround for unsupported keys?
// -> Should i do this, or just put in all the keys???
void Input_Init(InputState* states, unordered_map<string, string> keyMappings)
{
    InputStates = states;

    for (const auto& [key, value] : keyMappings)
    {
        KeyInput* input = FindInput(InputStates, key);
        if (!input)
        {
            Logf("Unused key mapping: %s", key.c_str());
            continue;
        }

        if (IsChar(value))
        {
            // VK_KEYCODES always use the upper case value!
            input->keycode = toupper(value[0]);
            input->key_name = value;
        }
        else
        {
            // TODO: convert the special names to something more meaningfull
            input->keycode = GetSpecialKeyCode(value);
            input->key_name = value;
        }
    }
}
