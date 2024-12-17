#include "../module.h"

using Action = void (*)();

struct ScheduleItem
{
    bool pending;

    float wait_time;
    float target_wait_time;

    Action action;
};

struct ScheduleQueue
{
    int max_size;
    int pending_count;
    ScheduleItem* items;
};
