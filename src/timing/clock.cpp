#include "module.h"

static Clock Measurer = {};

void StartTimer(Clock& timer)
{
    QueryPerformanceFrequency(&timer.frequency);
    QueryPerformanceCounter(&timer.last_time);
    QueryPerformanceCounter(&timer.start_time);

    // initial value can't be 0, take 60 FPS as guideline
    timer.delta_time_real = 1.f / 60;
    timer.time_counter = timer.delta_time_real;
}

void UpdateTimer(Clock& timer)
{
    LARGE_INTEGER current_time;
    QueryPerformanceCounter(&current_time);

    timer.delta_time_real = (float)(current_time.QuadPart -
                                    timer.last_time.QuadPart) /
                            timer.frequency.QuadPart;
    timer.last_time = current_time;
    timer.fps = 1 / timer.delta_time_real;
    timer.time_counter += timer.delta_time_real;
    timer.sim_time = timer.time_scale * timer.delta_time_real;
}

/**
 * Checks the time since the last update call, without modifying the timer
 * object
 * Returns the result in ms
 */
float QueryDeltaTime(Clock& timer)
{
    LARGE_INTEGER current_time;
    QueryPerformanceCounter(&current_time);
    float deltaTime = (float)(current_time.QuadPart -
                              timer.last_time.QuadPart) /
                      timer.frequency.QuadPart;
    return deltaTime * 1000.;
}

/**
 * Time since start (of the counter = program strat)
 * Returns the time in ms (integer value)
 * This roughly corresponds to program run time
 */
int QueryTimeSinceTimerStart(Clock& timer)
{
    LARGE_INTEGER current_time;
    QueryPerformanceCounter(&current_time);
    float deltaTime = (float)(current_time.QuadPart -
                              timer.start_time.QuadPart) /
                      timer.frequency.QuadPart;
    return (int)deltaTime * 1000;
}
void Measure_Start(Clock& timer)
{
    StartTimer(timer);
}

void Measure_Update(Clock& timer)
{
    UpdateTimer(timer);
}

float Measure_Elapsed(Clock& timer)
{
    return QueryDeltaTime(timer);
}

float Measure_SinceStart(Clock& timer)
{
    return QueryTimeSinceTimerStart(timer);
}

void Timer_Init()
{
    StartTimer(Timer);
}

/**
 * Updates the timer object to represent the new state
 */
void Timer_Update()
{
    UpdateTimer(Timer);
}
