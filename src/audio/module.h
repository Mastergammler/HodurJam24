#pragma once

#include "../io/module.h"
#include "../logging/module.h"
#include "../timing/module.h"

#include "imports.h"
#include "types.h"

static AudioMaster AudioDevice = {};

void Audio_Init();
void Audio_Dispose();

void LoadOggAsPcm(AudioData& audio, const string filePath);
void CreateVoiceForAudio(AudioData* audio, VoiceSettings& settings);
void PlayAudio(AudioData* audio,
               PlaybackSettings playback,
               bool clearQueue = true);
