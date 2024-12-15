#pragma once

#include "module.h"

#include "map/types.h"
#include "player/types.h"
#include "types.h"

static ExitCallback ExitFunction;
static HodurInputState GameInputs = {18, new KeyInput[18]};
static LoadedAudio Audio;
static Player Player;
static LevelState Level;

/**
 * Stereo voice playback for non directional things
 */
static VoiceSettings GlobalVoice;
