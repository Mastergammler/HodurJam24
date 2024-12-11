#include "imports.h"

#include "io/module.h"
#include "logging/module.h"
#include "timing/module.h"
#include "win32/module.h"

static const char DELIMITER = ':';
static const string INPUT_CONF = "configs/keyboard.conf";

static bool running = true;

void CloseProgram()
{
    running = false;
}

void LogCmdArgs()
{
    vector<string> cmdArgs = ReadArguments();
    for (int i = 0; i < cmdArgs.size(); i++)
    {
        Logf("Found argument: %s", cmdArgs[i].c_str());
    }
}

int WinMain(HINSTANCE instance,
            HINSTANCE prevInstance,
            LPSTR lpCmdLine,
            int nShowCmd)
{
    InitLogger();
    LogCmdArgs();

    HWND window = InitializeWindow("EscapeRoomGame", instance, CloseProgram);
    HDC hdc = GetDC(window);

    // TODO: should be controlled by the game
    unordered_map<string, string> keyMappings = read_kvp_file(INPUT_CONF,
                                                              DELIMITER);
    Timer_Init();
    Input_Init(keyMappings);

    Log("Hodur Jam 24");

    while (running)
    {
        Timer_Update();
        HandleMessages(window);
        // rendering
        WaitTillNextFrame(window);
    }

    DisposeLogger();
    return 0;
}
