#include "game/module.h"
#include "timing/module.h"

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
        Logf("Cmd argument: %s", cmdArgs[i].c_str());
    }
}

int WinMain(HINSTANCE instance,
            HINSTANCE prevInstance,
            LPSTR lpCmdLine,
            int nShowCmd)
{
    Logger_Init();
    timeBeginPeriod(1);
    Timer_Init();
    LogCmdArgs();

    HWND window = InitializeWindow("EscapeRoomGame", instance, CloseProgram);
    // HDC hdc = GetDC(window);

    Game_Init(CloseProgram);

    while (running)
    {
        Timer_Update();
        HandleMessages(window);
        Game_Update();
        // rendering
        WaitTillNextFrame(window);
    }

    Game_Dispose();
    timeEndPeriod(1);
    Logger_Dispose();
    return 0;
}
