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

void InitLogger(bool consoleInstantFlush = false);
void DisposeLogger();
void Log(string s);
void Logf(const string message, ...);
string format(const string message, ...);

// TODO: move somewhere else?! -> to win32?
//  void ReadArguments(LPWSTR* argList, int nArgs);
