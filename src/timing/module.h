#pragma once

#include "imports.h"
#include "types.h"

static Clock Timer = {};

void Timer_Init();
void Timer_Update();
void WaitTillNextFrame(HWND window);

void Measure_Start(Clock& timer);
void Measure_Update(Clock& timer);
float Measure_Elapsed(Clock& timer);
float Measure_SinceStart(Clock& timer);
