#include "module.h"
#include "queue.cpp"

void Audio_Update(float deltaTimeS)
{
    AudioQueueTick(deltaTimeS);
}
