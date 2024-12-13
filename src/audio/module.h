#pragma once

#include "../io/module.h"
#include "../logging/module.h"
#include "../timing/module.h"

#include "imports.h"
#include "types.h"

static AudioMaster AudioDevice = {};

void Audio_Init();
void Audio_Dispose();

void PlayNewAudio(AudioData* audio);
void LoadOggAsPcm(AudioData& audio, const string filePath);
