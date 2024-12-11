// TODO: should i also just do this via header files?
#pragma once

#include "../internal.h"

// TODO: use lower case names only?
// -> Adjust for map also?
string TrimToVariableName(string s)
{
    size_t pos = s.find_last_of('.');
    return pos == string::npos ? s : s.substr(pos + 1);
}
