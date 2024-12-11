#include "module.h"

#define NAMEOF(x) #x

KeyInput* FindInput(string name)
{
    for (auto& keyInput : Input.All)
    {
        if (keyInput.identifier == name) return &keyInput;
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
                                        {"QM", VK_OEM_2}};

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

// TODO: use lower case names only?
// -> Adjust for map also?
string TrimToVariableName(string s)
{
    size_t pos = s.find_last_of('.');
    return pos == string::npos ? s : s.substr(pos + 1);
}

void InitIdentifiers()
{
    // TODO: renames within the macro don't work -> bit annoying
    Input.Exit.identifier = TrimToVariableName(NAMEOF(Input.Exit));
    Input.Action.identifier = TrimToVariableName(NAMEOF(Input.Action));
    Input.TurnLeft.identifier = TrimToVariableName(NAMEOF(Input.TurnLeft));
    Input.TurnRight.identifier = TrimToVariableName(NAMEOF(Input.TurnRight));
    Input.MoveForward.identifier = TrimToVariableName(
                                            NAMEOF(Input.MoveForward));
    Input.MoveBackward.identifier = TrimToVariableName(
                                            NAMEOF(Input.MoveBackward));
    Input.MoveLeft.identifier = TrimToVariableName(NAMEOF(Input.MoveLeft));
    Input.MoveRight.identifier = TrimToVariableName(NAMEOF(Input.MoveRight));
    Input.Log.identifier = TrimToVariableName(NAMEOF(Input.Log));
    Input.Debug.identifier = TrimToVariableName(NAMEOF(Input.Debug));
    Input.Mixer.identifier = TrimToVariableName(NAMEOF(Input.Mixer));
    Input.Fire.identifier = TrimToVariableName(NAMEOF(Input.Fire));
    Input.Manual.identifier = TrimToVariableName(NAMEOF(Input.Manual));
    Input.Skip.identifier = TrimToVariableName(NAMEOF(Input.Skip));
}

// TODO: Feature: include keycode direct input -> that way it's a workaround for
// non supported special characters
void Input_Init(unordered_map<string, string> keyMappings)
{
    InitIdentifiers();

    for (const auto& [key, value] : keyMappings)
    {
        KeyInput* input = FindInput(key);
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
