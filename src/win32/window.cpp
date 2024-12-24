#include "imports.h"
#include "input.cpp"
#include "module.h"
#include <windef.h>
#include <wingdi.h>

// TODO: handle x-box-controller input

// NOTE: code from CaseyMuratori, not 100% sure whats going on here, or haw this
// works, just looks very complicated
#define X_INPUT_GET_STATE(name)                                                \
    DWORD WINAPI name(DWORD dwUserIndex, XINPUT_STATE* pState)
typedef X_INPUT_GET_STATE(x_input_get_state);
X_INPUT_GET_STATE(XInputGetStateStub)
{
    return ERROR_DEVICE_NOT_CONNECTED;
}
static x_input_get_state* XInputGetState_ = XInputGetStateStub;

static ExitCallback ExitProgram;

/**
 * Callback for window events
 */
LRESULT CALLBACK WindowCallback(HWND hwnd,
                                UINT uMsg,
                                WPARAM wParam,
                                LPARAM lParam)
{
    LRESULT result = 0;
    switch (uMsg)
    {
        case WM_CLOSE:
        case WM_DESTROY:
        {
            ExitProgram();
        }
        break;
        // NOTE: this event is called every time i touch the window
        case WM_GETMINMAXINFO:
        {
        }
        break;
        case WM_SETFOCUS:
        {
            HCURSOR hCursor = LoadCursor(NULL, IDC_IBEAM);
            SetCursor(hCursor);
        }
        break;
        case WM_SIZE: break;
        case WM_ACTIVATEAPP: OutputDebugStringA("WM_ACTIVEAPP\n"); break;
        // sys keydown is for things like Alt+F4
        case WM_SYSKEYDOWN:
        case WM_SYSKEYUP:
        {
            // handle as default right now
            result = DefWindowProc(hwnd, uMsg, wParam, lParam);
        }
        break;
        case WM_KEYUP:
        case WM_KEYDOWN:
        {
            HandleKeyboardInput(lParam, wParam);
        }
        break;
        default:
        {
            result = DefWindowProc(hwnd, uMsg, wParam, lParam);
        }
    }
    return result;
}

vector<string> ReadArguments()
{
    int nArgs;
    LPWSTR* argList = CommandLineToArgvW(GetCommandLineW(), &nArgs);

    vector<string> cmdArgs;

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
            cmdArgs.push_back(buffer);
        }
    }

    return cmdArgs;
}

HWND InitializeWindow(string name, HINSTANCE instance, ExitCallback exitProgram)
{

    ExitProgram = exitProgram;

    WNDCLASS winClass = {};
    winClass.style = CS_HREDRAW | CS_VREDRAW;
    winClass.lpszClassName = name.c_str();
    winClass.lpfnWndProc = WindowCallback;
    winClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);

    RegisterClass(&winClass);
    return CreateWindow(winClass.lpszClassName,
                        name.c_str(),
                        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                        CW_USEDEFAULT,
                        CW_USEDEFAULT,
                        800,
                        600,
                        // Screen.width,
                        //  38 is task bar height
                        // Screen.height + 38,
                        0,
                        0,
                        instance,
                        0);
}

/**
 * Without the default message handling the window will not respond
 * And you will not be able to close it
 */
void HandleMessages(HWND window)
{
    // we need to reset the single frame states before handling new messages
    // else these states could be persisted multiple rounds
    Input_ResetKeyStates();

    MSG message;
    while (PeekMessage(&message, window, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-variable"
void HandleController()
{
    // NOTE: for controller stuff use xinput

    // TODO: should we poll this more frequently
    for (DWORD controllerIndex = 0; controllerIndex < XUSER_MAX_COUNT;
         ++controllerIndex)
    {
        XINPUT_STATE controllerState;
        if (XInputGetState_(controllerIndex, &controllerState) == ERROR_SUCCESS)
        {
            // NOTE: controller is plugged in
            // TODO: See if controllerState.dwPacketNumber
            // increments too rapidly
            XINPUT_GAMEPAD* pad = &controllerState.Gamepad;

            bool up = (pad->wButtons & XINPUT_GAMEPAD_DPAD_UP);
            bool down = (pad->wButtons & XINPUT_GAMEPAD_DPAD_DOWN);
            bool left = (pad->wButtons & XINPUT_GAMEPAD_DPAD_LEFT);
            bool right = (pad->wButtons & XINPUT_GAMEPAD_DPAD_RIGHT);
            bool start = (pad->wButtons & XINPUT_GAMEPAD_START);
            bool back = (pad->wButtons & XINPUT_GAMEPAD_BACK);
            bool lb = (pad->wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER);
            bool rb = (pad->wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER);
            bool aBtn = (pad->wButtons & XINPUT_GAMEPAD_A);
            bool bBtn = (pad->wButtons & XINPUT_GAMEPAD_B);
            bool xBtn = (pad->wButtons & XINPUT_GAMEPAD_X);
            bool yBtn = (pad->wButtons & XINPUT_GAMEPAD_Y);

            int16_t stickX = pad->sThumbLX;
            int16_t stickY = pad->sThumbLY;

            if (aBtn)
            {
                // TODO: action
            }
        }
        else
        {
            // NOTE: controller is not available
        }
    }
}
#pragma clang diagnostic pop
