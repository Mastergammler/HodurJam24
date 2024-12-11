#include "clock.cpp"

/**
 * Actual restriction for the desired frame time
 * This controls the actual fps
 * TODO: these should not be hardcoded here
 */
float TargetFps = 144;
float TargetFrameTime = 1000. / TargetFps;

/**
 * Checks if it's time to show the new window text
 * Becaues changing this too often is hard to read, if we update it every frame
 */
bool ShouldUpdateFpsText()
{
    if (Timer.time_counter > Timer.fps_update_threshold)
    {
        Timer.time_counter -= Timer.fps_update_threshold;
        return true;
    }
    return false;
}

void WaitTillNextFrame(HWND window)
{
    float frameTimeMs = Timer.delta_time_real * 1000;

    if (ShouldUpdateFpsText())
    {
        stringstream ss;
        ss << setprecision(3) << Timer.fps << " FPS - " << frameTimeMs
           << " ms/f";

        std::string title = ss.str();
        SetWindowText(window, title.c_str());
    }

    if (frameTimeMs < TargetFrameTime)
    {
        DWORD timeToRest = (DWORD)(TargetFrameTime - frameTimeMs);
        if (timeToRest > 0)
        {
            Sleep(timeToRest);
        }
    }

    // further waiting if we're still not there
    // this waits in very small increments till we reach the actual threshold
    // this seems to lead to alot more consistent frame times
    while (QueryDeltaTime(Timer) < TargetFrameTime)
    {
        Sleep(0);
    }
}
