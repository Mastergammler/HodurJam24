#include "module.h"
#include "queue.cpp"
#include "xaudio2.cpp"

void Audio_Init()
{
    XAudio_Init();
    Init_Queue(8);
}

void Audio_Dispose()
{
    XAudio_Dispose();
}
