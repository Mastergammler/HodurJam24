#include "module.h"

static FILE* std_out = nullptr;
static FILE* std_err = nullptr;
// static FILE* std_in = nullptr;

/**
 * Attaches the output streams to the current console
 *
 * The reason i have to do this in the first place, is because winMain writes to
 * a differnt output stream by default, to the debug console usually, and not
 * the std out, and this is why it's not working?!
 */
void InitOutputStreams()
{
    // pipeing out to the console that starts it
    if (!AttachConsole(ATTACH_PARENT_PROCESS))
    {
        AllocConsole();
    }
    freopen_s(&std_out, "CONOUT$", "w", stdout);
    freopen_s(&std_err, "CONOUT$", "w", stderr);

    cout.clear();
    cerr.clear();
    wcout.clear();
    wcerr.clear();
}

/*void LogArguments(int argc, char* argv[])
{
    cout << "Program started with arguments:" << endl;

    for (int i = 0; i < argc; i++)
    {
        cout << "Found argument: " << argv[i] << endl;
    }

    cout << endl;
}

void ReadArguments(LPWSTR* argList, int nArgs)
{
    CmdArgs = new string[nArgs];
    CmdArgCount = nArgs;

    if (argList)
    {
        for (int i = 0; i < nArgs; i++)
        {
            // max arg length
            char buffer[256];
            WideCharToMultiByte(CP_ACP,
                                0,
                                argList[i],
                                -1,
                                buffer,
                                sizeof(buffer),
                                NULL,
                                NULL);
            Logf("Found argument: %s", buffer);
            CmdArgs[i] = buffer;
        }
        cout << endl;
    }
}*/
