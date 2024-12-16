// necessary because startup & update both call this
#pragma once

#include "module.h"

// TODO: put this on it's own thread? Does this make sense?
// -> For small things this is clearly not necessary
DelayedPlaybackQueue DelayQueue = {};

void Init_Queue(int maxSize)
{
    DelayQueue.items = new QueueItem[maxSize];
    DelayQueue.size = maxSize;
    DelayQueue.pending_count = 0;
    memset(DelayQueue.items, 0, sizeof(QueueItem) * maxSize);
}

void AudioQueue_ClearSchedule()
{
    for (int i = 0; i < DelayQueue.size; i++)
    {
        QueueItem* cur = &DelayQueue.items[i];
        cur->pending = false;
    }
    DelayQueue.pending_count = 0;
}

void SchedulePlayback(AudioData* audio,
                      PlaybackSettings settings,
                      float delayInS)
{
    if (delayInS <= 0)
    {
        PlayAudio(audio, settings);
        return;
    }

    if (DelayQueue.pending_count >= DelayQueue.size)
    {
        Logf("Queueing audio %s skipped, because queue is full!",
             audio->file_name.c_str());
        return;
    }

    // PERF: could optimize for max queued items, but i think YAGNI now
    // -> this queue is probably short enough that it doesn't matter
    for (int i = 0; i < DelayQueue.size; i++)
    {
        QueueItem* cur = &DelayQueue.items[i];
        if (cur->pending) continue;

        cur->pending = true;
        cur->audio = audio;
        cur->settings = settings;
        cur->remaining_wait_time = delayInS;
        DelayQueue.pending_count++;

        return;
    }
}

void AudioQueueTick(float deltaTimeS)
{
    if (DelayQueue.pending_count == 0) return;

    for (int i = 0; i < DelayQueue.size; i++)
    {
        QueueItem* cur = &DelayQueue.items[i];
        if (!cur->pending) continue;

        cur->remaining_wait_time -= deltaTimeS;

        if (cur->remaining_wait_time <= 0)
        {
            cur->pending = false;
            DelayQueue.pending_count--;
            PlayAudio(cur->audio, cur->settings);
        }
    }
}
