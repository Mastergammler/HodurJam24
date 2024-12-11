#pragma once

#include "module.h"

bool VerifyFile(ifstream& file, string path)
{
    if (!file.is_open())
    {
        char buffer[MAX_PATH];
        GetCurrentDirectoryA(MAX_PATH, buffer);
        Logf("Did not find file '%s'. Current dir '%s'", path.c_str(), buffer);
        return false;
    }
    return true;
}
