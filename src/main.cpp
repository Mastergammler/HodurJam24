#include "imports.h"
#include "logging/module.h"

int WinMain(HINSTANCE instance,
            HINSTANCE prevInstance,
            LPSTR lpCmdLine,
            int nShowCmd)
{
    InitLogger();

    Log("Hodur Jam 24");

    DisposeLogger();
    return 0;
}
