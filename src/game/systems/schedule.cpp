#include "../internal.h"

static ScheduleQueue Queue;

void ScheduleExecution(float delayInS, Action action)
{
    if (Queue.pending_count < Queue.max_size)
    {
        for (int i = 0; i < Queue.max_size; i++)
        {
            ScheduleItem* cur = &Queue.items[i];
            if (cur->pending) continue;

            cur->pending = true;
            cur->wait_time = 0;
            cur->target_wait_time = delayInS;
            cur->action = action;
            break;
        }
    }
    else
    {
        Log("Unable to schedule item, because queue is full!");
    }
}

void Schedule_Init(int queueSize)
{
    Queue.max_size = queueSize;
    Queue.items = new ScheduleItem[Queue.max_size];

    memset(Queue.items, 0, sizeof(ScheduleItem) * Queue.max_size);
}

void Schedule_Update(float deltaTime)
{
    for (int i = 0; i < Queue.max_size; i++)
    {
        ScheduleItem* cur = &Queue.items[i];
        if (!cur->pending) continue;

        cur->wait_time += deltaTime;
        if (cur->wait_time >= cur->target_wait_time)
        {
            cur->action();
            Queue.pending_count--;
            cur->pending = false;
        }
    }
}

void Schedule_Clear()
{
    for (int i = 0; i < Queue.max_size; i++)
    {
        Queue.items[i].pending = false;
    }

    Queue.pending_count = 0;
}
