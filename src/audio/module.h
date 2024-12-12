#pragma once

#include "../logging/module.h"

#include "imports.h"
#include "types.h"

static AudioMaster Mixer = {};

void Audio_Init();
void Audio_Dispose();

void PlayTest(const string filePath);
