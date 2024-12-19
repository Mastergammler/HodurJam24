#pragma once

#include "imports.h"

struct Logger
{
    ofstream log_file;
    vector<string> log_buffer;
    mutex log_mutex;
    condition_variable log_condition;
    thread log_thread;

    bool running;
};

void Logger_Init(bool consoleInstantFlush = false);
void Logger_Dispose();

// FIXME: It seems like, that writing 3 log messages costs about 1ms frame time
// - this is extremely poor, logging should not affect the game loop at all
// I need to investigate why this is the case, and prevent this from happening
// -> The console  could be disconnected from the game thread completely right?
// => So maybe i can separate those completely?
void Log(string s);
void Logf(const string message, ...);
string format(const string message, ...);
